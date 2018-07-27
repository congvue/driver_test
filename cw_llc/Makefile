TARGET = cw-llc

$(TARGET)-objs := llc-api.o \
                  llc-usb.o \
                  llc-device.o \
                  llc-thread.o \
                  llc-list.o \
                  llc-msg.o \
                  llc-monitor.o \
                  llc-netdev.o \
                  llc-firmware.o \
                  llc-spi.o \
                  llc-module.o

obj-m	:= $(TARGET).o


KDIR := /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/


all:
	make -C $(KDIR) M=`pwd` modules 


clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
