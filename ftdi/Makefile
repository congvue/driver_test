obj-m    += ftdi_sio.o usb-serial.o generic.o bus.o

KDIR    := /home/wangyf/work/qy_board_kernel_imx_yygd/
#PWD    := $(shell pwd)

all:
#$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	make -C $(KDIR) M=`pwd` modules


clean:
	rm -rf *.o
	rm -rf *.ko



