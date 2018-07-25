#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

#define NAME	"/dev/vir_ts"
#define MS 		10

//#define UP

int main()
{
	int fd ;
	char buffer1[15]={};
	int ret;
	int i,j;
	char buffer2[15] = {};

	fd = open(NAME, O_RDWR);
	if(fd < 0){
		printf("open %s failed\n",NAME);
		return fd;
	}
	

	for(j=0;j<1000;j+=50){
		for(i=0;i<800;i++){
			
			snprintf(buffer2, 15,"%d,%d,1",200+i+j ,i+100);

			usleep(MS  * 1000);
			//scanf("%s",buffer);
			
			ret = write(fd, buffer2,strlen(buffer2));
			if(ret < 0){
				printf("write file failed\n");
				return ret;
			}
	}

#ifdef UP
			usleep(MS  * 1000);
			snprintf(buffer1, 15,"300,%d,0",i+300);
			printf("Time:%s buffer1 = %s\n",__TIME__,buffer1);
						
			ret = write(fd, buffer1,strlen(buffer1));
			if(ret < 0){
				printf("write file failed\n");
				return ret;
			}
#endif
		}
	//}

	close(fd);

	return 0;

		

}
