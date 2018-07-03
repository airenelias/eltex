#include "variables.h"
int main()
{
	extern int e;
	printf("Extern variable = %d\n",e);
	for(int i=0;i<10;i=i+1)
	printf("Result = %d\n",add(i,i));
	return 0;
}
