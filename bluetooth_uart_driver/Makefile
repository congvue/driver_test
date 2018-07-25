obj-m		:= hci_uart.o
hci_uart-objs	:= hci_ldisc.o hci_h4.o hci_rtk_h5.o rtk_coex.o

KDIR := /home/wangyuanfei/L3.0.35_E9/linux-3.0.35

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.mod.c *.mod.o *.ko *.symvers *.order *.a
