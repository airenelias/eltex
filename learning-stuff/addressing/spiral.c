#include <stdio.h>
#define N 5

int main()
{
	int i, j;
	int A[N][N];
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
		while(1)
		{
			for(j=lb;j<rb;j=j+1,i=i+1)
				A[tb][j]=i;
			tb=tb+1;
			if(tb==bb) break;

			for(j=tb;j<bb-1;j=j+1,i=i+1)
				A[j][rb-1]=i;
			rb = rb-1;
			if(rb==lb) break;

			for(j=rb;j>=lb;j=j-1,i=i+1)
				A[bb-1][j]=i;
			bb=bb-1;
			if(bb==tb) break;

			for(j=bb-1;j>=tb;j=j-1,i=i+1)
				A[j][lb]=i;
			lb=lb+1;
			if(lb==rb) break;
		}
	}

	for(i=0;i<N;i=i+1)
	{
		for(j=0;j<N;j=j+1)
			printf("%2d ",A[i][j]);
		printf("\n");
	}
	return 0;
}
