#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>

#define 		NETLINK_TEST		17
#define 		MAX_PAYLOAD			1024


struct sockaddr 	src_addr, dest_addr;
struct nlmsghdr		*nlh 	= NULL;
struct iovec 		iov;
int 				sock_fd;
struct msghdr		msg;

int main(int argc ,char **argv)
{
	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_SIZE);

	memset(&src_addr, 0, sizeof(src_addr));

	memset(&msg, 0, sizeof(msg));

	src_addr.nl_family 		= AF_NETLINK;
	src_addr.nl_pid			= getpid();		//self pid
	src_addr.nl_groups		= 1;

	bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

	memset(&dest_addr, 0, sizeof(dest_addr));

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));

	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

	iov.iov_base		= (void *)nlh;
	iov.iov_len			= NLMSG_SPACE(MAX_PAYLOAD);

	msg.msg_name		= (void *)&dest_addr;
	msg.msg_namelen		= sizeof(dest_addr);
	msg.msg_iov			= &iov;
	msg.msg_iovlen		= 1;

	printf("waiting for message from kernel\n");

	recvmsg(sock_fd, &msg, 0);

	printf("Received message payload :%s\n",NLMSG_DATA(nlh));

	close(sock_fd);

}