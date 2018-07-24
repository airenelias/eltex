#include "./libkeys.h"

void*callgetnumname(void *num)
{
	int *intnum = num;
	char *buf = getnumname(*intnum);
	printf("%s", buf);
}

int main()
{
	int i;
	int n=1;
	pthread_t tid[100];
	int nums[5] = {0,1,2,3,4};
	for(i=0;i<100;i++,n++)
	{
		if(i%10 == 0) n = 0;
		else n = 1;
		pthread_create(&tid[i], NULL, &callgetnumname, &nums[n]);
	}
	for(i=0;i<100;i++)
	{
		pthread_join(tid[i], NULL);
	}
	return 0;
}