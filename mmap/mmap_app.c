#if 1

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char *addr = NULL;

	fd  =  open("/dev/mmap",O_RDWR);
	if(fd < 0) {
		printf("open fail\n");
		return -1;
	}

	addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if(addr  == MAP_FAILED) {
		printf("mmap fail\n");
		return -1;
	}

	printf("%s\n",addr);
	memset(addr, 'f', 100);
	addr[0]  = 'p';
	printf("%s\n",addr);

	munmap(addr, 4096);

	addr = NULL;
	close(fd);


	fd = open("/dev/mmap",O_RDWR);
	if(fd < 0) {
		return -1;
	}

	addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if(addr == MAP_FAILED) {
		printf("map fail\n");
		return -2;
	}

	printf("%s\n",addr);
	munmap(addr, 4096);

	close (fd);

	return 0;

}

#endif

