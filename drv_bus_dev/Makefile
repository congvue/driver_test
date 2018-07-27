obj-m +=helloword.o 


KDIR :=/home/launch/samba/wangyuanfei/fsl-release-bsp/test2_yocto/tmp/work/imx6sxsabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
