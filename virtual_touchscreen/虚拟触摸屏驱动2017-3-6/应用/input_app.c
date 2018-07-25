#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>


#define INPUT_NAME	"/dev/input/event0"
#define VIRTUAL_TS  "/dev/virtual_tp"

int pendown = 0;
int get_x = 0;
int get_y = 0;


int main()
{
	int fd,ret, virtual_ts_fd;

	int value_x, value_y, prev_x, prev_y, next_x, next_y;
	
	struct input_event data;
	unsigned char buff[10];
	
	fd = open(INPUT_NAME, O_RDONLY);
	if(fd < 0){
		printf("open %s failed\n",INPUT_NAME);
		return fd;
	}

	virtual_ts_fd = open(VIRTUAL_TS, O_RDWR);
	if(virtual_ts_fd < 0){
		printf("open virtual ts failed\n");
		return -1;
	}
	
	while(1) {
		printf("read data .............................\n");

		memset(&data, 0, sizeof(data));

		ret = read(fd, &data, sizeof(data));
		if(ret < 0){
			printf("read %s fail\n",INPUT_NAME);
			return ret;
		}
#if 1
		if((data.type == EV_ABS)&&(data.code == ABS_PRESSURE) && (data.value == 255)){
			printf("Pendownd = 1.....\n");
			pendown = 1;
		}

		if((data.type == EV_ABS)&&(data.code == ABS_PRESSURE) && (data.value == 0)){
			printf("Pendownd = 0.....\n");
			pendown = 0;
		}
#endif		

		if((data.type == EV_ABS)&&(data.code == ABS_X)){
			printf("X--- = %d\n",data.value);
			prev_x = data.value;
			get_x = 1;
		}
		
		if((data.type == EV_ABS)&&(data.code == ABS_Y)){
			printf("Y ---- = %d\n",data.value);
			prev_y = data.value;
			get_y = 1;
		}

		if((EV_SYN == data.type)&&(1 == pendown)){
			printf("Write down event .....\n");
			snprintf(buff, 10, "%hd,%hd,%d",prev_x,prev_y,1);
			printf("buff = %s\n",buff);
			ret = write(virtual_ts_fd, buff, strlen(buff));
			if(ret < 0){
				printf("write /dev/virtual_tp fail\n");
				break;
			}
		}

		if((EV_SYN == data.type)&&(0 == pendown)){
			printf("Write UP event .....\n");
			snprintf(buff, 10, "%hd,%hd,%hd",prev_x,prev_y,0);
			printf("buff = %s\n",buff);
			ret = write(virtual_ts_fd, buff, strlen(buff));
			if(ret < 0){
				printf("write /dev/virtual_tp fail\n");
				break;
			}
			ret = write(virtual_ts_fd, buff, strlen(buff));
			if(ret < 0){
				printf("write /dev/virtual_tp fail\n");
				break;
			}
		}
		
#if 0
		if((get_x == 1)&&(get_y == 1)){
			printf("begint to write down event\n");
			snprintf(buff, 10, "%d,%d,%d",prev_x,prev_y,1);
			ret = write(virtual_ts_fd, buff, strlen(buff));
			if(ret < 0){
				printf("write /dev/virtual_tp fail\n");
				break;
			}

			printf("begint to write up event\n");
			snprintf(buff, 10, "%d,%d,%d",prev_x,prev_y,0);
			ret = write(virtual_ts_fd, buff, strlen(buff));
			if(ret < 0){
				printf("write /dev/virtual_tp fail\n");
				break;
			}

			get_x = 0;
			get_y = 0;
	
	
		}
#endif		


	}

	close(fd);
	close(virtual_ts_fd);

#if 0

		ret = read(fd, &data, sizeof(data));
		if(ret < 0){
			printf("read %s failed\n ",INPUT_NAME);
			return ret;
		}

		if(data.type == EV_KEY) {
			
			printf("type : 	EV_KEY, event = %s, VALUE = 0x%x\n",data.code == BTN_TOUCH ? "BTN_TOUCH" : "UNKNOW", data.value);
		
		} else if(data.type == EV_ABS) {
			printf("type: EV_ABS event = %s, value = %d\n", 
					data.code == ABS_X ? "ABS_X" :
					data.code == ABS_Y ? "ABS_Y":
					data.code == ABS_PRESSURE ? "ABS_PRESSURE" :
					"UNKNOW", data.value); 
		}else if(data.type == EV_SYN) {
		
			printf("type:EV_SYN, event = %s, value = 0x%x\n",data.code == SYN_REPORT ? "SYN_REPORT" : "UNKNOW", data.value);

		}else  {
		
			printf("type : 0x%x, event = 0x%x, value = 0x%x\n",data.type, data.code,data.value);
		}

		printf("\n\n");
#endif		


	return 0;

}
