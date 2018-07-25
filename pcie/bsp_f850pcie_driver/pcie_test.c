#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


#define READ_STATE   _IOW('p', 0x01, int)
#define WRITE_STATE  _IOW('p', 0x02, int) 
#define SET_FPGA_REG _IOW('p', 0x03, int)
#define GET_FPGA_REG _IOW('p', 0x04, int)
#define WRITE_BUF_LOCK          0
#define WRITE_BUF_SYNC          1
#define READ_BUF_LOCK           2

struct reginfo{
        unsigned addr;
        unsigned int value;
};

static const char * g_fpga_dev[] = {
	"/dev/pcie_endpoint1",
	"/dev/pcie_endpoint2",	
};

int main(int argc, char **argv)
{
	int fd_dev, ret;
	unsigned int device = 0;
	unsigned int type  = 0;
	unsigned int length = 10;
	unsigned int times  = 1;
	int  opt;

	while ((opt = getopt(argc, argv, "d:t:l:h:")) != -1) 
	{
		switch (opt) 
		{
			case 'd':
				device = strtoul(optarg, NULL, 10);
				break;
			case 't':
				type = strtoul(optarg, NULL, 16);
				break;
			case 'l':
				length = strtoul(optarg, NULL, 10);
				break;
			case 'h':
				times = strtoul(optarg, NULL, 10);
				break;
			default:
				break;
		}
	}
	printf("device = %d, type = 0x%x,len = %d, times = %d,\n", device, type, length, times);

	fd_dev = open(g_fpga_dev[device], O_RDWR, 0);
	if (fd_dev == -1)
	{
		printf("Failed to open device, path=%s.\n", g_fpga_dev[device]);
		return -1;
	}
	//======================================================
	struct reginfo reg = {
		.addr = 0x114,
		.value = 0xAA,
	};

	struct reginfo reg1 = {
		.addr = 0x114,
	};

	// set reg to pcie_dev
	ret = ioctl(fd_dev, SET_FPGA_REG, &reg);
	printf("Set fpga reg: %d\n", ret);

	// get reg from pcie_dev
	ret = ioctl(fd_dev, GET_FPGA_REG, &reg1);
	printf("get fpga reg: %d, value: %x\n", ret, reg1.value);
	//======================================================

	close(fd_dev);

	printf("EXIT\n"); 
	return 0;
}

