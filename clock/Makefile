obj-m +=clock.o 


KDIR :=/home/wangyuanfei/L4.1.15_E9/e9_v2/kernel-4.1.15
#KDIR :=/home/wangyuanfei/wangyuanfei/anpark/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
