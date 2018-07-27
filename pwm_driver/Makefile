obj-m +=pwm_test.o
#/home/launch/samba/wangyuanfei/fsl-release-bsp/test2_yocto/tmp/sysroots/x86_64-linux/usr/bin/arm-poky-linux-gnueabi

KDIR :=  /home/launch/samba/wangyuanfei/fsl-release-bsp/test_yoct/tmp/work/imx6sxsabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/ 
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
