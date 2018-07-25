
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if.h>
#include <linux/if_arp.h>
//#define VPNET_DEBUG
//#define VPNET_IP_LAYER
typedef struct vpnet
{
	spinlock_t              lock;
	struct net_device_stats stats;
	struct sk_buff_head     readq;
	wait_queue_head_t       rwait;
	struct net_device      *dev;
	unsigned int            flags;
	#define VPNET_FLAG_IP_LAYER 0x00008000
} vpnet_t;
/* --- Declarations start --- */
static ssize_t vpnet_chr_read(struct file *file, char __user *data, size_t count, loff_t *f_pos);
static ssize_t vpnet_chr_write(struct file *file, const char __user *data, size_t count, loff_t *f_pos);
static int vpnet_chr_open(struct inode *inode, struct file *file);
static int vpnet_chr_release(struct inode *inode, struct file *file);
static int vpnet_chr_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
static int vpnet_start_xmit(struct sk_buff *skb, struct net_device *dev);
static int vpnet_open(struct net_device *dev);
static int vpnet_stop(struct net_device *dev);
static int vpnet_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
static unsigned int vpnet_chr_poll(struct file *file, poll_table *wait);
static struct net_device_stats *vpnet_stats(struct net_device *dev);
static void vpnet_tx_timeout(struct net_device *dev);
static int vpnet_set_mac_address(struct net_device *dev, void *addr);
static int vpnet_change_mtu(struct net_device *dev, int mtu);
static int vpnet_init(struct net_device *dev);
static void vpnet_setup(struct net_device *dev);
/* --- Declarations end --- */
/* Character device part::: */
static const struct file_operations vpnet_fops = 
{
	.owner   = THIS_MODULE,
	.ioctl   = vpnet_chr_ioctl,
	.read    = vpnet_chr_read,
	.write   = vpnet_chr_write,
	.open    = vpnet_chr_open,
	.poll    = vpnet_chr_poll,
	.release = vpnet_chr_release,
};
static ssize_t vpnet_chr_read(struct file *file, char __user *data, size_t count, loff_t *f_pos)
{
	vpnet_t *vpnet = (vpnet_t *)file->private_data;
	DECLARE_WAITQUEUE(wait, current);
	struct sk_buff *skb;
	ssize_t len = -1;
	unsigned long ret;
	
	add_wait_queue(&vpnet->rwait, &wait);
	for(;;)
	{
		set_current_state(TASK_INTERRUPTIBLE);
		if(file->f_flags & O_NONBLOCK)
		{
			len = -EAGAIN;
			break;
		}
		/* Read packets from queue */
		if((skb = skb_dequeue(&vpnet->readq)))
		{
			len = skb->len;
			ret = copy_to_user(data, skb->data, len);
			vpnet->stats.tx_packets++;
			vpnet->stats.tx_bytes += len;
#ifdef VPNET_DEBUG
			printk("---- READ: %d ----/n", len);
#endif
			/* Wake up transmit queue and free packet */
			netif_wake_queue(vpnet->dev);
			dev_kfree_skb(skb);
			break;
		}
		//
		if(signal_pending(current))
		{
			len = -ERESTARTSYS;
			break;
		}
		/* Nothing to do, let it sleep */
		schedule();
	}
	
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&vpnet->rwait, &wait);
	return len;
}
static ssize_t vpnet_chr_write(struct file *file, const char __user *data, size_t count, loff_t *f_pos)
{
	vpnet_t *vpnet = (vpnet_t *)file->private_data;
	struct sk_buff *skb;
	unsigned long ret;
	
	if(count < 0)
		return -EINVAL;
	if(count == 0)
		return 0;
	
	/* Allocate buffer for received frame data */
	if((skb = dev_alloc_skb(count + 4)) == NULL)
	{
		vpnet->stats.rx_errors++;
		return -EINVAL;
	}
	/* Fill `sk_buff` with frame data */
	skb->dev = vpnet->dev;
	skb_reserve(skb, 2);
	ret = copy_from_user(skb_put(skb, count), data, count);
	if(vpnet->flags & VPNET_FLAG_IP_LAYER)
	{
		switch(skb->data[0] & 0xf0)
		{
		case 0x40:
			skb->protocol = htons(ETH_P_IP);
			break;
		case 0x60:
			skb->protocol = htons(ETH_P_IPV6);
			break;
		default:
			vpnet->stats.rx_dropped++;
			kfree_skb(skb);
			return -EINVAL;
		}
	}
	else
	{
		skb->protocol = eth_type_trans(skb, vpnet->dev);
	}
	/* Notify the higher level with it... */
	netif_rx(skb);
	/* Update statistics */
	vpnet->dev->last_rx = jiffies;
	vpnet->stats.rx_packets++;
	vpnet->stats.rx_bytes += count;
	
	return count;
}
static int vpnet_chr_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}
static unsigned int vpnet_chr_poll(struct file *file, poll_table *wait)
{
	vpnet_t *vpnet = (vpnet_t *)file->private_data;
	unsigned int mask = POLLOUT | POLLWRNORM;
	//printk(">>---  ");
	poll_wait(file, &vpnet->rwait, wait);
	//printk("---<</n");
	if(skb_queue_len(&vpnet->readq) > 0)
		mask |= POLLIN | POLLRDNORM;
	return mask;
}
static int vpnet_chr_open(struct inode *inode, struct file *file)
{
	vpnet_t *vpnet = NULL;
	struct net_device *dev;
	
	/* Allocate a private device structure */
	dev = alloc_netdev(sizeof(vpnet_t), "vpnet%d", vpnet_setup);
	if(dev==NULL)
	{
		printk(KERN_ERR "%s[%d] alloc_netdev() error./n", __FILE__, __LINE__);
		return -1;
	}
	dev->init  = vpnet_init;
	/* Set private data */
	vpnet = (vpnet_t *)netdev_priv(dev);
	memset(vpnet, 0x0, sizeof(vpnet_t));
	file->private_data = (void *)vpnet;
	vpnet->dev = dev;
#ifdef VPNET_IP_LAYER
	vpnet->flags |= VPNET_FLAG_IP_LAYER;
#endif
	/* Register device to kernel */
	if(register_netdev(dev) != 0)
	{
		free_netdev(dev);
		printk(KERN_ERR "%s[%d] register_netdev() error./n", __FILE__, __LINE__);
		return -1;
	}
	spin_lock_init(&vpnet->lock);
	/* Initialize read q & wait q */
	skb_queue_head_init(&vpnet->readq);
	init_waitqueue_head(&vpnet->rwait);
	
	return 0;
}
static int vpnet_chr_release(struct inode *inode, struct file *file)
{
	vpnet_t *vpnet = (vpnet_t *)file->private_data;
	/* Clear read queue */
	skb_queue_purge(&vpnet->readq);
	/* Unregister netdevice and release used private data */
	unregister_netdev(vpnet->dev);
	free_netdev(vpnet->dev); vpnet->dev = NULL;
	
	return 0;
}
/* Network device part::: */
static int vpnet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	vpnet_t *vpnet = (vpnet_t *)netdev_priv(dev);
	
#ifdef VPNET_DEBUG
	printk(KERN_INFO "%s[%d]: skb->len: %d/n", __FILE__, __LINE__, skb->len);
#endif
	if(skb_queue_len(&vpnet->readq) >= dev->tx_queue_len)
	{
		vpnet->stats.tx_fifo_errors++;
		vpnet->stats.tx_dropped++;
		kfree_skb(skb);
#ifdef VPNET_DEBUG
		printk("%s[%d] qlen: %d, tx_queue_len: %d/n", __FILE__, __LINE__,
			(int)skb_queue_len(&vpnet->readq), (int)dev->tx_queue_len);
#endif
		return 0;
	}
	netif_stop_queue(dev);
	spin_lock(&vpnet->lock);
	/* Add current `sk_buff` packet to the char device's read queue */
	skb_queue_tail(&vpnet->readq, skb);
	dev->trans_start = jiffies;
	spin_unlock(&vpnet->lock);
	/* Notify and wake up the reader process */
	netif_wake_queue(dev);
	wake_up_interruptible(&vpnet->rwait);
	return 0;
}
static int vpnet_open(struct net_device *dev)
{
	netif_start_queue(dev);
	return 0;
}
static int vpnet_stop(struct net_device *dev)
{
	netif_stop_queue(dev);
	return 0;
}
static int vpnet_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	return 0;
}
static struct net_device_stats *vpnet_stats(struct net_device *dev)
{
	vpnet_t *vpnet = netdev_priv(dev);
	return &vpnet->stats;
}
static void vpnet_tx_timeout(struct net_device *dev)
{
	netif_wake_queue(dev);
}
static int vpnet_set_mac_address(struct net_device *dev, void *addr)
{
	return 0;
}
static int vpnet_change_mtu(struct net_device *dev, int mtu)
{
	if(mtu < 68 || mtu + dev->hard_header_len > 65535 - 20 - 8)
		return -EINVAL;
	dev->mtu = mtu;
	return 0;
}
static int vpnet_init(struct net_device *dev)
{
	vpnet_t *vpnet = (vpnet_t *)netdev_priv(dev);
	/* Set options for P-t-P/Ethernet device */
	if(vpnet->flags & VPNET_FLAG_IP_LAYER)
	{
		dev->mtu             = 1500;
		dev->hard_header_len = 0;
		dev->addr_len        = 0;
		dev->type            = ARPHRD_NONE;
		dev->flags           = IFF_POINTOPOINT | IFF_NOARP | IFF_MULTICAST;
		dev->tx_queue_len    = 100;
	}
	else
	{
		ether_setup(dev);
		random_ether_addr(dev->dev_addr);
		dev->tx_queue_len    = 100;
	}
	
	return 0;
}
static void vpnet_setup(struct net_device *dev)
{
	/* Setup the operation handlers */
	dev->open            = vpnet_open;
	dev->stop            = vpnet_stop;
	dev->do_ioctl        = vpnet_ioctl;
	dev->get_stats       = vpnet_stats;
	dev->hard_start_xmit = vpnet_start_xmit;
	dev->tx_timeout      = vpnet_tx_timeout;
	dev->set_mac_address = vpnet_set_mac_address;
	dev->watchdog_timeo  = 3 * HZ;
	dev->change_mtu      = vpnet_change_mtu;
}
#define MODULE_NAME   "vpnet"
#define DEVICE_NAME   "vpnet"
#define DEVICE_MAJOR  200
#define DEVICE_MINOR  0
static struct class *vpnet_class = NULL;
int  __init vpnet_module_init(void)
{
	if(register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &vpnet_fops) < 0)
	{
		printk(KERN_ERR "%s[%d] register_chrdev() error./n", __FILE__, __LINE__);
		return -1;
	}
	/* Create dirs and files under /sys */
	if((vpnet_class = class_create(THIS_MODULE, MODULE_NAME)) == NULL)
	{
		printk(KERN_ERR "%s[%d] class_create() error./n", __FILE__, __LINE__);
		unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
		return -1;
	}
	if(device_create(vpnet_class, NULL, MKDEV(DEVICE_MAJOR, DEVICE_MINOR),
		"%s", DEVICE_NAME) == NULL)
	{
		printk(KERN_ERR "%s[%d] device_create() error./n", __FILE__, __LINE__);
		class_destroy(vpnet_class);
		vpnet_class = NULL;
		unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
		return -1;
	}
	//
	printk(KERN_INFO "Just-VPN driver, v0.9, Jianying Liu <rssn@163.com>/n");
	//
	return 0;
}
void __exit vpnet_module_exit(void)
{
	device_destroy(vpnet_class, MKDEV(DEVICE_MAJOR, DEVICE_MINOR));
	class_destroy(vpnet_class);
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
}
module_init(vpnet_module_init);
module_exit(vpnet_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jianying Liu <rssn@163.com>");
MODULE_DESCRIPTION("Just-VPN Network driver, v0.9");
//</pre>

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#define VPNET_DEBUG
#define BOOL  int
#define TRUE  1
#define FALSE 0
static unsigned long domain_aton(const char * s)
{
	unsigned long rv;
	rv = inet_addr(s);
	if(rv==0xffffffff)
	{
		struct hostent * he;
		he = (struct hostent *)gethostbyname(s);
		if(he)
			return ((struct in_addr *)he->h_addr_list[0])->s_addr;
		else
			return 0xffffffff;
	}
	else
		return rv;
}
static void echo_help(int argc, char *argv[])
{
	printf("Just-VPN for VPN driver test, v0.90/n");
	printf("Usage: %s -s [-p <local_port>]      : Run as server mode/n", argv[0]);
	printf("       %s <host_addr> [host_port]   : Run as client mode/n", argv[0]);
	printf("       %s -h                        : Print this help/n", argv[0]);
	printf("/n");
}
int main(int argc, char *argv[])
{
	char rbuf[1024*4];
	const size_t rbuf_sz = sizeof(rbuf);
	int vfd;
	int sfd;
	fd_set rset;
	int rx, tx;
	int ret, i;
	int port = 9000;
	char *host = NULL;
	struct sockaddr_in peer_addr, lsn_addr;
	int peer_addr_len, lsn_addr_len;
	BOOL is_server_mode = FALSE;
	int opt;
	
	memset(&peer_addr, 0x0, sizeof(peer_addr));
	memset(&lsn_addr, 0x0, sizeof(lsn_addr));
	while( (opt = getopt(argc, argv, "slcp:h-")) != -1)
	{
		switch(opt)
		{
		case 'c':
			is_server_mode = FALSE;
			break;
		case 's':
		case 'l':
			is_server_mode = TRUE;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'h':
		case '-':
			echo_help(argc, argv);
			return 0;
			break;
		case '?':
			echo_help(argc, argv);
			return -1;
			break;
		}
	}
	
	/* Hostname or dotted-IP of peer host */
	if(optind < argc)
		host = argv[optind++];
	/* Listen port of peer host (UDP) */
	if(optind < argc && !is_server_mode)
		port = atoi(argv[optind++]);
	/* Open device file, while opening the interface is created */
	vfd = open("/dev/vpnet", O_RDWR);
	if(vfd < 0)
	{
		fprintf(stderr, "open() error: %s./n", strerror(errno));
		return -1;
	}
	/* Create data transmission socket */
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd < 0)
	{
		fprintf(stderr, "socket() error: %s./n", strerror(errno));
		return -1;
	}
	if(is_server_mode)
	{
		lsn_addr.sin_family = AF_INET;
		lsn_addr.sin_addr.s_addr = htonl(0);
		lsn_addr.sin_port = htons(port);
		ret = bind(sfd, (struct sockaddr *)&lsn_addr, sizeof(lsn_addr));
		if(ret<0)
		{
			fprintf(stderr, "%s[%d] bind() error: %s./n", __FILE__, __LINE__, strerror(errno));
			return -1;
		}
		printf("Just-VPN, listening on `%s:%d`.../n",
			inet_ntoa(lsn_addr.sin_addr),
			(int)ntohs(lsn_addr.sin_port) );
	}
	else
	{
		if(host==NULL)
		{
			fprintf(stderr, "%s[%d] Invalid IP address./n", __FILE__, __LINE__);
			return -1;
		}
		peer_addr.sin_family = AF_INET;
		peer_addr.sin_addr.s_addr = domain_aton(host);
		peer_addr.sin_port = htons(port);
		printf("Just-VPN, connecting with `%s:%d`.../n",
			inet_ntoa(peer_addr.sin_addr),
			(int)ntohs(peer_addr.sin_port) );
	}
	
	/* Forward each frame data packet */
	for(;;)
	{
		FD_ZERO(&rset);
		FD_SET(vfd, &rset);
		FD_SET(sfd, &rset);
		ret = select((sfd>vfd ? sfd : vfd) + 1, &rset, NULL, NULL, NULL);
		if(ret<0)
			continue;
		else if(ret==0)
			continue;
		
		if(FD_ISSET(sfd, &rset))
		{
			/* For the server, we need this to know where the client is;
			   while for the client, this address may changed after 
			   received this packet, so risk of attack occurs here.  */
			peer_addr_len = sizeof(peer_addr);
			rx = recvfrom(sfd, rbuf, rbuf_sz, 0,
				(struct sockaddr *)&peer_addr, &peer_addr_len);
			if(rx > 0)
			{
				tx = write(vfd, rbuf, (size_t)rx);
				//printf("recvfrom()->write(): %d/n", tx);
			}
		}
		
		if(FD_ISSET(vfd, &rset))
		{
			rx = read(vfd, rbuf, rbuf_sz);
			/* For the server, before the client sends packet to it, 
			   we cannot know where to send local frame packets to */
			if(rx>0 && peer_addr.sin_addr.s_addr != 0 && peer_addr.sin_port != 0)
			{
				tx = sendto(sfd, rbuf, rx, 0,
					(struct sockaddr *)&peer_addr, sizeof(peer_addr));
				//printf("read()->sendto(): %d/n", tx);
			}
		}
	}
	
	return 0;
}</pre>
<p><br>
?</p>
<p>?</p>
<p>Makefile£º</p>
<p>?</p>
<p>?</p>
<pre class="cpp" name="code"># Driver module
MODULE_NAME   := vpnet
KERNSRC       ?= $(shell pwd)/../linux-2.6.27-x86
#KERNSRC      ?= /usr/src/linux-headers-$(shell uname -r)
$(CC)         ?= gcc
obj-m := $(MODULE_NAME).o
all: $(MODULE_NAME).ko just-vpn
$(MODULE_NAME).ko: $(MODULE_NAME).c
	make -C $(KERNSRC) M=$(shell pwd) modules
just-vpn: just_vpn.c
	$(CC) -o $@ {1}lt;
clean:
	find . /( -name '.tmp_versions' -o -name '*.o' -o -name '*.ko' -o -name '.*.cmd' /
		-o -name '.*.d' -o -name '*.mod.c' -o -name '*.symvers' -o -name '*~' /) -print | xargs rm -fr;
	rm -f just-vpn</pre>
<p><br>
</p>
<p>?</p>
<p>?</p>
<p>?</p>
   

#endif
