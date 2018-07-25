obj-m +=mmap.o 


KDIR :=  /home/wangyf/work/linux-3.0.35 
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
