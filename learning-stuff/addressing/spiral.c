#include <stdio.h>
#define N 5

int main()
{
	int i, j;
	int A[N][N];
	int B[N][N];
	for(i=0;i<N;i=i+1)
		for(j=0;j<N;j=j+1)
			A[i][j]=i*N+j;

	for(i=0;i<N;i=i+1)
	{
		for(j=0;j<N;j=j+1)
			printf("%2d ",A[i][j]);
		printf("\n");
	}

	{
		int tb=0, bb=N, lb=0, rb=N;
		i=0;
		int *p;
		p=&A[0][0];
		while(1)
		{
			for(j=lb;j<rb;j=j+1,i=i+1)
				B[tb][j]=*(p+i);
			tb=tb+1;
			if(tb==bb) break;

			for(j=tb;j<bb-1;j=j+1,i=i+1)
				B[j][rb-1]=*(p+i);
			rb = rb-1;
			if(rb==lb) break;

			for(j=rb;j>=lb;j=j-1,i=i+1)
				B[bb-1][j]=*(p+i);
			bb=bb-1;
			if(bb==tb) break;

			for(j=bb-1;j>=tb;j=j-1,i=i+1)
				B[j][lb]=*(p+i);
			lb=lb+1;
			if(lb==rb) break;
		}
	}

	for(i=0;i<N;i=i+1)
	{
		for(j=0;j<N;j=j+1)
			printf("%2d ",B[i][j]);
		printf("\n");
	}
	return 0;
}
