#include <string.h>
#include <stdlib.h>

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
