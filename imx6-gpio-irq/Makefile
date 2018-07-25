obj-m +=gpio_test.o


#KDIR :=/home/pjl/Public/wangyuanfei/e9/kernel
KDIR :=/home/wangyuanfei/L4.1.15_E9/e9_v2/kernel-4.1.15
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
