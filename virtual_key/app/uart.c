#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>

#include <sys/ioctl.h>
//#include <serial_test.h>

typedef struct {
    unsigned int lable;
	unsigned int baudrate;
} SERIAL_BAUD_ST;

typedef struct{
    unsigned int flags;
	unsigned int delay_rts_after_send;	/* delay after send (milliseconds) */
}SERIAL_RS485_CONF_ST;

typedef struct {
    char parity;
	unsigned int baud;
    unsigned int databits;
    unsigned int stopbits;
} SERIAL_ATTR_ST;

#define TIMEOUT                     1  /* read operation timeout 1s = TIMEOUT/10 */
#define MIN_LEN                     128  /* the min len datas */
#define DEV_NAME_LEN                20
#define SERIAL_ATTR_BAUD            115200
#define SERIAL_ATTR_DATABITS        8
#define SERIAL_ATTR_STOPBITS        1
#define SERIAL_ATTR_PARITY          'n'
#define SERIAL_MODE_NORMAL          0
#define SERIAL_MODE_485             1
#define DELAY_RTS_AFTER_SEND        1   /* 1ms */
#define SER_RS485_ENABLED		    1	/* 485 enable */

#define RET_SUCCESS                 0
#define RET_FAILED                  -1
#define BUF_SIZE                    512



static SERIAL_BAUD_ST g_attr_baud[] = {
    {230400, B230400}, {115200, B115200},
    {57600, B57600}, {38400, B38400}, 
    {19200, B19200}, {9600, B9600},
    {4800, B4800},  {2400, B2400},
    {1800, B1800}, {1200, B1200},
};

static char g_dev_serial[][DEV_NAME_LEN] = {
		"/dev/ttymxc0",
        "/dev/ttymxc1",
        "/dev/ttymxc2",
        "/dev/ttymxc3",
        "/dev/ttymxc4",
};


#define DEBUG_INFO(fmt, args...)  fprintf(stderr, fmt, ##args) 
#define DEBUG_ERROR(fmt, args...) fprintf(stderr, fmt"%s\t%s\t%d\n", ##args, __FILE__,__FUNCTION__,__LINE__)


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


static int attr_baud_set(int fd, unsigned int baud)
{
    int i; 
    int ret = RET_SUCCESS; 
    struct termios option;

    /* get old serial attribute */
    memset(&option, 0, sizeof(option));
    if (RET_SUCCESS != tcgetattr(fd, &option)) 
    { 
        DEBUG_ERROR("tcgetattr failed.\n");
        return RET_FAILED;  
    }
    
    for (i = 0; i < ARRAY_SIZE(g_attr_baud);  i++) 
	{ 
        if (baud == g_attr_baud[i].lable) 
        {     
            ret = tcflush(fd, TCIOFLUSH);
            if (RET_SUCCESS != ret)
            {
                DEBUG_ERROR("tcflush failed.\n");
                break;
            }
            
            ret = cfsetispeed(&option, g_attr_baud[i].baudrate); 
            if (RET_SUCCESS != ret)
            {
                DEBUG_ERROR("cfsetispeed failed.\n");
                ret = RET_FAILED;
                break;
            }
            
            ret = cfsetospeed(&option, g_attr_baud[i].baudrate); 
            if (RET_SUCCESS != ret)
            {
                DEBUG_ERROR("cfsetospeed failed.\n");
                ret = RET_FAILED;
                break;
            }
            
            ret = tcsetattr(fd, TCSANOW, &option);  
            if  (RET_SUCCESS != ret) 
            {        
                DEBUG_ERROR("tcsetattr failed.\n");
                ret = RET_FAILED;
                break;     
            }    
            
            ret = tcflush(fd, TCIOFLUSH);
            if (RET_SUCCESS != ret)
            {
                DEBUG_ERROR("tcflush failed.\n");
                break;
            }
        }  
    }
    
    return ret;
}

static int attr_other_set(int fd, SERIAL_ATTR_ST *serial_attr)
{ 
	struct termios option;	

    /* get old serial attribute */
    memset(&option, 0, sizeof(option));
    if (RET_SUCCESS != tcgetattr(fd, &option)) 
    { 
        DEBUG_ERROR("tcgetattr failed.\n");
        return RET_FAILED;  
    }

    option.c_iflag = CLOCAL | CREAD;
    
    /* set datas size */
    option.c_cflag &= ~CSIZE; 
    option.c_iflag = 0;

    switch (serial_attr->databits)
    {   
        case 7:		
            option.c_cflag |= CS7; 
            break;
            
        case 8:     
            option.c_cflag |= CS8;
            break;  
            
        default:    
            DEBUG_ERROR("invalid argument, unsupport datas size.\n");
            return RET_FAILED;  
    }

    /* set parity */    
    switch (serial_attr->parity) 
    {   
        case 'n':
        case 'N':    
            option.c_cflag &= ~PARENB;   
            option.c_iflag &= ~INPCK;      
            break;  
            
        case 'o':   
        case 'O':    
            option.c_cflag |= (PARODD | PARENB);  
            option.c_iflag |= INPCK;            
            break;  
            
        case 'e':  
        case 'E':   
            option.c_cflag |= PARENB;       
            option.c_cflag &= ~PARODD;   
            option.c_iflag |= INPCK;     
            break;
            
        case 's': 
        case 'S':  
            option.c_cflag &= ~PARENB;
            option.c_cflag &= ~CSTOPB;
            break;  
            
        default:   
            DEBUG_ERROR("invalid argument, unsupport parity type.\n");
            return RET_FAILED;  
    }  
    
    /* set stop bits  */
    switch (serial_attr->stopbits)
    {   
        case 1:    
            option.c_cflag &= ~CSTOPB;  
            break;  
            
        case 2:    
            option.c_cflag |= CSTOPB;  
            break;
            
        default:    
            DEBUG_ERROR("invalid argument, unsupport stop bits.\n");
            return RET_FAILED; 
    } 
    
    option.c_oflag = 0;
    option.c_lflag = 0;  
    option.c_cc[VTIME] = TIMEOUT;    
    option.c_cc[VMIN] = MIN_LEN; 

    if (RET_SUCCESS != tcflush(fd,TCIFLUSH))   
    { 
        DEBUG_ERROR("tcflush failed.\n");
        return RET_FAILED;  
    }
    
    if (RET_SUCCESS != tcsetattr(fd, TCSANOW, &option))   
    { 
        DEBUG_ERROR("tcsetattr failed.\n");
        return RET_FAILED;  
    }

    return RET_SUCCESS;  
}

static int attr_set(int fd, SERIAL_ATTR_ST *serial_attr)
{
    int ret = RET_SUCCESS;
    
	if (NULL == serial_attr)
	{
        DEBUG_ERROR("invalid argument.\n");
        return RET_FAILED;  
	}
    
    if (RET_SUCCESS == ret)
    {
        ret = attr_baud_set(fd, serial_attr->baud);
        if (RET_SUCCESS == ret)
        {
            ret = attr_other_set(fd, serial_attr);
        }
    }

    return ret;
}


void print_hex(void *data, int size)
{
	int i;
	for(i=0;i<size;i++){
		printf("0x%2x ",*(unsigned char *)data);
		data++;
	}
	printf("\n");
	
}

unsigned char cmd_left1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0x81, 0xd0};
unsigned char cmd_left2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x16, 0x46};

unsigned char cmd_right1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0x80, 0xd1};
unsigned char cmd_right2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x0d, 0x5d};

unsigned char cmd_up1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0x85, 0xd4};
unsigned char cmd_up2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x15, 0x45};

unsigned char cmd_down1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0x86, 0xd7};
unsigned char cmd_down2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x18, 0x48};

unsigned char cmd_enter1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0xce, 0x9f};
unsigned char cmd_enter2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x07, 0x57};

unsigned char cmd_esc1[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x01, 0x82, 0xd3};
unsigned char cmd_esc2[8] = {0xaa, 0x75, 0x8d, 0x00, 0x02, 0x00, 0x09, 0x59};


int main(int argc, char **argv)
{
	int fd;
	int fd_tp;
	int bufsize;
	char write_buf[BUF_SIZE];
	char buf[20] = "/dev/ttymxc0";

	memcpy(buf+11, argv[1], 1);
	printf("buf = %s\n",buf);
	

	fd = open(buf, O_RDWR);
    if (fd < 0) 
	{
		printf("open uart fail\n");
        return RET_FAILED;
	}
	fd_tp = open("/dev/virtual_key",O_RDWR);
	if(fd_tp<0){
		printf("open virtual tp fail\n");
		return -1;
	}
	
	int ret;
	SERIAL_ATTR_ST serial_attr;

    memset(&serial_attr, 0, sizeof(serial_attr));
    serial_attr.baud = SERIAL_ATTR_BAUD;
    serial_attr.databits = SERIAL_ATTR_DATABITS;
    serial_attr.stopbits = SERIAL_ATTR_STOPBITS;
    serial_attr.parity = SERIAL_ATTR_PARITY;

    ret = attr_set(fd, &serial_attr);
	
	char read_buf[20];
	
	while(1){
		ret = read(fd, read_buf, sizeof(read_buf));
		if((ret >0) && !memcmp(read_buf, cmd_left1, 8) || !memcmp(read_buf,cmd_left2, 8)){
			//report left key
			printf("report left key\n");
			write(fd_tp, "105", 4);
		}
		
		if((ret >0) && !memcmp(read_buf, cmd_right1, 8) || !memcmp(read_buf,cmd_right2, 8)){
			//report left key
			printf("report right key\n");
			write(fd_tp, "106", 4);
		}
		
		if((ret >0) && !memcmp(read_buf, cmd_up1, 8) || !memcmp(read_buf,cmd_up2, 8)){
			//report left key
			printf("report up key\n");
			write(fd_tp, "103", 4);
		}
		
		if((ret >0) && !memcmp(read_buf, cmd_down1, 8) || !memcmp(read_buf,cmd_down2, 8)){
			//report left key
			printf("report down key\n");
			write(fd_tp, "108", 4);
		}
		
		if((ret >0) && !memcmp(read_buf, cmd_enter1, 8) || !memcmp(read_buf,cmd_enter2, 8)){
			//report left key
			printf("report enter key\n");
			write(fd_tp, "28", 3);
		}
		
		if((ret >0) && !memcmp(read_buf, cmd_esc1, 8) || !memcmp(read_buf,cmd_esc2, 8)){
			//report left key
			printf("report esc key\n");
			write(fd_tp, "1", 2);
		}
			
		//print_hex(read_buf, ret);
		
		
	}
		


    return 0 ;
}
