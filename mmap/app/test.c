#if 1

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE	5 * 1024 

int main()
{
	int fd;
	char *addr = NULL;

	fd  =  open("/dev/mmap",O_RDWR);
	if(fd < 0) {
		printf("open fail\n");
		return -1;
	}

	addr = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	printf("###########SIZE = %d\n",SIZE);

	if(addr  == MAP_FAILED) {
		printf("mmap fail\n");
		return -1;
	}

	

	printf("readk:%s read over\n",addr);
	printf("ssss&adddr = 0x%p\n",addr);
	memset(addr, 'f', SIZE);

	

	munmap(addr, SIZE);

	addr = NULL;
	close(fd);


	fd = open("/dev/mmap",O_RDWR);
	if(fd < 0) {
		return -1;
	}

	addr = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if(addr == MAP_FAILED) {
		printf("map fail\n");
		return -2;
	}

	printf("%s\n",addr);
	munmap(addr, SIZE);

	close (fd);

	return 0;

}

#endif

