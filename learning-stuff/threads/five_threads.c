#include <stdio.h>
#include <pthread.h>

void *func(void *ptr)
{
	int *thread_ptr = ptr;
	printf("%d\n", *thread_ptr);
	//return 0;
}

int main()
{
	pthread_t tid[5];
	int arr[5] = {1,2,3,4,5};
	int i = 0;
	for(i = 0; i<5; i++)
	{
		pthread_create(&tid[i], NULL, &func, &arr[i]);
	}
	for(i = 0; i<5; i++)
		pthread_join(tid[i], NULL);
	return 0;
}