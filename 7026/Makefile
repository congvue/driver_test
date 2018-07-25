obj-m +=mxcfb_ch7026.o 


KDIR :=/home/pjl/Public/wangyuanfei/anpark/kernel2/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
