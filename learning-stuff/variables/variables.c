#include "variables.h"
int e = 6;
int add(int a, int b)
{
	static int i;
        i = i+1;
	printf("Iteration = %d\n",i);
	return a+b;
}
