#include <malloc.h>
#include <pthread.h>
#include <string.h>

/*char buf[8];
char *nums[5] = {"Z", "o", "two", "three", "four"};
char *getnumname(int num)
{
	strncpy(buf, nums[num], 8);
	return buf;
}*/

pthread_key_t key;
pthread_once_t once = PTHREAD_ONCE_INIT;

void dest(void *buf)
{
	free(buf);
}

void key_cre()
{
	pthread_key_create(&key, dest);
}

char *nums[5] = {"Z", "o", "w", "h", "f"};

char *getnumname(int num)
{
	char *buf;
	pthread_once (&once, key_cre);
	buf = pthread_getspecific(key);
	if(buf==NULL)
	{
		buf = malloc(8);
		pthread_setspecific(key, buf);
	}
	strncpy(buf, nums[num], 8);
	return buf;
}