obj-m +=kobj.o 


KDIR :=  /home/wangyf/work/qy_board_kernel_imx_yygd 
all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
