obj-m +=kobj_uvent.o


KDIR :=/home/wangyuanfei/L3.0.35_E9/linux-3.0.35/
#KDIR :=/home/wangyuanfei/wangyuanfei/anpark/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
