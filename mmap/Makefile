export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

obj-m +=mmap.o 




#KDIR :=/home/wangyuanfei/L3.0.35_E9/linux-3.0.35/
KDIR :=/home/wangyuanfei/L4.1.15_E9/e9_v2/kernel-4.1.15
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
