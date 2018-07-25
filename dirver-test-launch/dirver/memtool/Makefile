memtool:memtool.c arg_env.c
	arm-poky-linux-gnueabi-gcc -mhard-float -g -c -o arg_env.o arg_env.c
	arm-poky-linux-gnueabi-gcc -mhard-float -g -c -o memtool.o memtool.c
	arm-poky-linux-gnueabi-gcc -o memtool arg_env.o memtool.o
clean:
	rm -rf memtool *.o
