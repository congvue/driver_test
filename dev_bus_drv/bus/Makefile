obj-m +=bus.o 


#KDIR :=/home/pjl/Public/wangyuanfei/e9/kernel
KDIR := /home/pjl/Public/wangyuanfei/anpark/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
