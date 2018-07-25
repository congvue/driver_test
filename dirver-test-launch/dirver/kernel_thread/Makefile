obj-m +=helloword.o 


KDIR :=  /home/wangyf/work/new_linux3.0.35/linux-3.0.35 
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
