#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>


#define 		MAX_PAYLOAD			1024
#define 		NETLINK_TEST		17

struct sock 	*nl_sk = NULL;


void nl_data_ready(struct sock *sk, int len)
{
	wake_up_interruptible(sk->sk_sleep);

}

void test_netlink(void)
{
	struct sk_buff *skb 	= NULL;
	struct nlmsghdr *nlh;

	nl_sk = netlink_kernel_create(NETLINK_TEST, nl_data_ready);

	skb   = alloc_skb(NLMSG_SPACE(MAX_PAYLOAD), GFP_KERNEL);

	nlh	  = (struct nlmsghdr *)skb->data;

	nlh->nlmsg_len		= NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid		= 0;		//from kernel
	nlh->nlmsg_flags	= 0;

	nlh = (struct nlmsghdr *)skb_put(skb, NLMSG_SPACE(MAX_PAYLOAD));

	strcpy(NLMSG_DATA(nlh), "Greeting from kernel!");

	//sender is in group 
	NETLINK_CB(skb).groups 		= 1;
	NETLINK_CB(skb).pid 		= 0;//from kernel
	NETLINK_CB(skb).dst_pid 	= 0;//multicast

	//to mcast group
	NETLINK_CB(skb).dst_groups  = 1;

	//multicast the message to all listering processes
	netlink_broadcast(nl_sk, skb, 0, 1, GFP_KERNEL);
	printk("%s\n",NLMSG_DATA(nlh));

	sock_release(nl_sk->sk_socket);
	
}

int init_module()
{
	test_netlink();

	return 0;

}


void cleanup_module()
{

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
