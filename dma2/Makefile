export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

obj-m +=dma2.o




#KDIR :=/home/wangyuanfei/L3.0.35_E9/linux-3.0.35/
KDIR :=/home/percherry/Public/wangyuanfei/e9v3_ori/tmp
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
