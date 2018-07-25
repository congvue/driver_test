CC = arm-fsl-linux-gnueabi-gcc 

test: test.o
	$(CC) -s test.o -o test   -lpthread
test.o:test.c
	$(CC) -c test.c -o test.o -lpthread

clean:
	rm *.o test 
