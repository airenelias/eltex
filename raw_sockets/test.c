#include <stdio.h>
#include <malloc.h>
typedef struct {
	unsigned short int a:8;
	unsigned short int b:8;
} test;

int main()
{
	short int a = 511;
	test *s = &a;
	printf("%d", s->b);
	return 0;
}