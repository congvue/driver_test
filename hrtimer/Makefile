obj-m +=hrtimer.o

KDIR := /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
