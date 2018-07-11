#include "calc.h"

int main()
{
	char operation;
	float re=0; float im=0;
	struct complex number = {0,0};
	struct complex *ptr = &number;
	printf("Input format: \"Operation Real Imaginary\", q to exit.\nExamples: \"+ 2 2\" or \"* 3 4\".\n");
	while(1)
	{
		printf("%f+%fi\n", ptr->re,ptr->im);
		scanf(" %c",&operation);
		if(operation=='q') break;
		scanf(" %f %f",&re,&im);
		switch(operation)
		{
			case '+': add(ptr,re,im); break;
			case '-': sub(ptr,re,im); break;
			case '*': mul(ptr,re,im); break;
			case '/': div(ptr,re,im); break;

			default: break;
		}
	}
	return 0;
}