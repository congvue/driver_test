obj-m +=virtual_ts.o 


KDIR :=/home/pjl/Public/wangyuanfei/e9/kernel
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
