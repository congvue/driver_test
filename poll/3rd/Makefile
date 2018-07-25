obj-m += test.o

KDIR :=/home/pjl/Public/wangyuanfei/beagleboneblack/yocto/poky-daisy/build/tmp/work/beaglebone-poky-linux-gnueabi/linux-yocto/3.14.4+gitAUTOINC+183622e809_0143c6ebb4-r0/linux
all:
	make -C $(KDIR) M=`pwd` modules
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
