#include <stdio.h>
#define N 10
#define array_even {0,1,2,3,4,5,6,7,8,9}
int main()
{
	int A[N]=array_even;
	
	printf("\n");
	for(int i=0;i<N;i=i+1)
		printf("%d ",A[i]);
	{
		int temp;
		for(int i=0;i<(int)(N/2);i=i+1)
		{
			temp = A[i];
			A[i]=A[(N-1)-i];
			A[(N-1)-i]=temp;
		}
	}
	printf("\n");
	for(int i=0;i<N;i=i+1)
		printf("%d ",A[i]);
	return 0;
}
