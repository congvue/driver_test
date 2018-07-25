obj-m +=virtual_key.o 


KDIR :=/home/wangyuanfei/wangyuanfei/anpark/linux-3.0.35
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
