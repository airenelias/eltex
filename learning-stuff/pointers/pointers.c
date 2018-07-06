#include <stdio.h>
void swap(int* ptr1, int* ptr2)
{
	ptr1=ptr2;
	printf("Func ptr = %p\n",ptr1);
}
int main()
{
	int a = 7;
	int b = 13;
	int *ptr=&a;
	printf("Main1ptr = %p\n",ptr);
	swap(ptr,&b);
	printf("Main2ptr = %p\n",ptr);
	return 0;
}
