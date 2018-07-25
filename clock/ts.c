#include <stdio.h>
#define ALIGN(a,size) ((a+size-1)&(~(size-1)))


void main()
{
	int a=5;
	int *p = &a;

	printf(" p = 0x%x, &p=0x%x, *p=0x%x\n",p,&p,*p);
	printf(" p = %p, &p=%p, *p=0x%x\n",p,&p,*p);

	printf("ALLIGN(15,128) = %d\n",ALIGN(15,128));
	printf("-128 = 0x%x\n",-128);
	printf("~(size-1)=%d\n",~(128-1));
	printf("142&(-128)=%d\n",142&(-128));
	printf("ALLIGN(225,128) = %d\n",ALIGN(225,128));

}
