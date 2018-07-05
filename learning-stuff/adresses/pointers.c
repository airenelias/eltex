#include <stdio.h>

void main()
{
	short *p;
	short A[3] = {1,2,3};
	p=&A[0];
	printf("&A=%p, p=%p\nA[0]=%d, p[0]=%d\n",&A,p,A[0],*p);
	printf("A[1]=%d, p[1]=%d\n",A[1],*(p+1));
}
