obj-m +=netlink_test.o 


KDIR :=/home/pjl/Public/wangyuanfei/e9/linux_E9_3.0.35_for_Linux
#KDIR :=/home/pjl/Public/wangyuanfei/anpark/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
