#include <unistd.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>



#define MEMDEV "/dev/mem"
#define MEMOFFSET 512

unsigned int hex(char *str)
{
    unsigned ret = 0;
    int i = 0;

    if((str[0] != '0') || ((str[1] != 'x') && (str[1] != 'X')))
    {	
        printf("str error!\n");
        return -1;
    }

    for(i = 0; i < (strlen(str) - 2); i++)
    {
        if((str[2 + i] >= '0') && (str[2 + i] <= '9'))
        {
            ret += str[2 + i] - '0';
        }
        else if((str[2 + i] >= 'a') && (str[2 + i] <= 'f'))
        {
            //printf("detect a - f\n");
            ret += 0xA + str[2 + i] - 'a';
        }
        else if((str[2 + i] >= 'A') && (str[2 + i] <= 'F'))
        {
            //printf("detect A - F\n");
            ret += 0xA + str[2 + i] - 'A';
        }
	else
        {
            //printf("detect NONE\n");
            ret += 0x0;
        }
        if(i < (strlen(str) - 3))
        ret <<= 0x4;
    }
    //printf("ret = 0x%08x, strlen = %d\n", ret, strlen(str));
 
    return ret;
}


static int dump(unsigned int phy_addr, unsigned int addr, unsigned int len)
{
    int i = 0;

    for(i = 0; i < (len >> 2); i++)
    {
        if((i % 4) == 0)
        {
             printf("\n0x%08x:", phy_addr);
        }
        printf("%08x ", *(unsigned int *)addr);
        addr += sizeof(int);
        phy_addr += sizeof(int);
    }
    printf("\n");

    return 0;
}

int main(int argc, char **argv)
{
    int m_fd = -1;
    int vir_addr = -1;
    unsigned int start_phy = 0x0;
    unsigned int len_vir   = 0x0; 
    unsigned int value     = 0x0;

    if((argc < 2) || (argc >3))
    {
        printf("Usage:\n\t%s PHY_ADDR\n", argv[0]);
        return -1;
    }
    
    start_phy = hex(argv[1]);
    if(start_phy == -1)
    {
        printf("phy_addr error!\n");
        return -1;
    }
#if 0
    len_vir   = hex(argv[2]);    
    if(len_vir == -1)
    {
        printf("len error!\n");
        return -1;
    }
#endif
    //printf("start_phy = 0x%08x,len_vir = 0x%08x\n", start_phy, len_vir);
    m_fd = open(MEMDEV, O_RDWR, 0655);
    if (m_fd < 0)
    {
        printf("open memory error!\n");
        return -1;
    }
    vir_addr = mmap((void*)0, MEMOFFSET, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, start_phy);
    if(vir_addr == -1)
    {
        printf("mmap failed !\n");
        goto err;
    }
    if(argc == 2)
    {
        dump(start_phy, vir_addr, 0x100);
    }
    else if(argc == 3)
    {
        value = hex(argv[2]);
        printf("0x%08x:====>0x%08x\n", start_phy, value);
        *(unsigned int *)vir_addr = value;
    }
err:
    close(m_fd);

    return 0;
}
