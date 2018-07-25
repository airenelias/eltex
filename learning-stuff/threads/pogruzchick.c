#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define CUSTOMER_COUNT 3
#define SHOP_SIZE 5

int Shop[SHOP_SIZE];
pthread_mutex_t mut[SHOP_SIZE];

void *watcher(void *needs_void)
{
	int i;
	int *needs = needs_void;
	system("clear");
	while(1) {
		printf("SHOP:");
		for(i = 0; i<SHOP_SIZE; i++)
			printf("%5d", Shop[i]);
		printf("\nCUSTOMERS:\n");
		for(i = 0; i<CUSTOMER_COUNT; i++)
			printf("%d\n", needs[i]);
		usleep(50000);
		system("clear");
	}
}

void *loader(void* arg)
{
	while(1) {
		int shop = rand()%SHOP_SIZE;
		if(pthread_mutex_trylock(&mut[shop]) == 0)
		{
			Shop[shop] += 500;
			pthread_mutex_unlock(&mut[shop]);
			sleep(2);
		}
	}
	return 0;
}

void *customer(void *need_void)
{
	int *need = need_void;
	while(*need > 0) {
		int shop = rand()%SHOP_SIZE;
		if(pthread_mutex_trylock(&mut[shop]) == 0)
		{
			*need -= Shop[shop];
			if(*need < 0)
			{
				Shop[shop] = abs(*need);
				*need = 0;
			}
			else
				Shop[shop] = 0;
			pthread_mutex_unlock(&mut[shop]);
			sleep(1);
		}
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	pthread_t tid_loader, tid_watcher;
	pthread_t tid_customer[CUSTOMER_COUNT];
	int needs[CUSTOMER_COUNT];
	int i, rnd;
	
	for(i = 0; i<SHOP_SIZE; i++) {
		Shop[i] = rand()%1100+900;
	}
	pthread_create(&tid_loader, NULL, &loader, NULL);
	usleep(500000);
	
	for(i = 0; i<CUSTOMER_COUNT; i++) {
		needs[i] = rand()%11000+9000;
		pthread_create(&tid_customer[i], NULL, &customer, &needs[i]);
	}
	pthread_create(&tid_watcher, NULL, &watcher, &needs);

	for(i = 0; i<CUSTOMER_COUNT; i++) {
		pthread_join(tid_customer[i], NULL);
	}
	pthread_cancel(tid_loader);
	pthread_cancel(tid_watcher);
	return 0;
}