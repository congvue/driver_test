#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/if_arp.h>

typedef struct vpnet {
	spinlock_t					lock;
	struct net_device_stats 	stats;
	struct sk_buff_head			readq;
	wait_queue_head_t			rwait;
	struct net_device			*dev;
	unsigned int 				flags;
	#define VMIC_FLAG_IP_LAYER	0X00008000
};

/*********************Network device part**********************/
static int vinc_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct vnic *vnic = (struct vnic*)netdev_priv(dev);

#ifdef VMIC_DEBUG
	printk(KERN_INFO "%s[%d]:skb->len:%d\n",__FILE__,__LINE__,(int)skb_queue_len(&vnic->readq),(int)dev-tx_queue_len);
#endif

	if(skb_queue_len(&vnic->readq) >= dev->tx_queue_len){
		vnic->stats.tx_fifo_errors++;
		

}