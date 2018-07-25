#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>


#define INPUT_NAME	"/dev/input/event1"

int main()
{
	int fd,ret;

	
	struct input_event data;
	unsigned char buff[10];
	
	fd = open(INPUT_NAME, O_RDONLY);
	if(fd < 0){
		printf("open %s failed\n",INPUT_NAME);
		return fd;
	}


	
	while(1) {
		printf("read data .............................\n");

		memset(&data, 0, sizeof(data));

		ret = read(fd, &data, sizeof(data));
		if(ret < 0){
			printf("read %s fail\n",INPUT_NAME);
			return ret;
		}
		
		printf("type = 0x%2x , code  = 0x%2x,  value = 0x%2x\n",data.type, data.code, data.value);





		if((data.type == EV_ABS)&&(data.code == ABS_X)){
			printf("X--- = %d\n",data.value);

		}
		
		if((data.type == EV_ABS)&&(data.code == ABS_Y)){
			printf("Y ---- = %d\n",data.value);

		}

		
		if(EV_KEY == data.type){
			if(data.code == KEY_ENTER){
				printf("ENTER KEY PRESS\n");
			}
			if(data.code == KEY_ESC){
				printf("KEY_ESCAPE KEY PRESS\n");
			}
			
			if(data.code == KEY_UP){
				printf("UP KEY PRESS\n");
			}
			if(data.value == KEY_RIGHT){
				printf("--> KEY PRESS\n");
			}
			
			if(data.code == KEY_DOWN){
				printf("DOWN KEY PRESS\n");
			}
			if(data.code == KEY_LEFT){
				printf("<-- LEFT KEY PRESS\n");
			}
		
		}

	}

	close(fd);
	return 0;

}
