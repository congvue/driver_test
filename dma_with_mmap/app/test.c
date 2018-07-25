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

	fd  =  open("/dev/dma_with_mmap",O_RDWR);
	if(fd < 0) {
		printf("open fail\n");
		return -1;
	}

	
	addr = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//printf("###########SIZE = %d\n",SIZE);


	char buffer[100];

	while(1){
		scanf("%s",buffer);

		write(fd, buffer,strlen(buffer));

		printf("readk:%s\n",addr);
	}


	//munmap(addr, SIZE);

	addr = NULL;

	close(fd);




}

#endif

