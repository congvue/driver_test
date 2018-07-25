#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#define 	GET_XTIME	0

void main(void)
{
	unsigned long now = 0;
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	printf("tv.tv_sec = %ld\n",tv.tv_sec);
	printf("tv.tv_usec = %ld\n",tv.tv_usec);


	int fd = open("/dev/xtime_dev", O_RDWR);
	if(fd < 0){
		printf("Open fail\n");
		return ;
	}


	ioctl(fd, GET_XTIME, &now);

	printf("xtime.tv_sec = %ld\n",now);

	close(fd);
}
