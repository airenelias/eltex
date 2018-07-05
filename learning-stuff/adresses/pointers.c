#include <stdio.h>

int main()
{
	int *p;
	int A[3] = {1,2,3};
	p=&A[0];
	printf("&a=%p, p=%p\na[0]=%d, p[0]=%d\n",&A,p,A[0],*p);
	printf("a[1]=%d, p[1]=%d\n",A[1],*(p+1));
	
	return 0;
}
