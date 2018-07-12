#include "../include/calc.h"
#include <dlfcn.h>
#include <malloc.h>
#include <string.h>

int main()
{
	int operation;
	int i = 0;
	float re=0; float im=0;
	struct complex number = {0,0};
	struct complex *ptr = &number;

	char **libraries;
	int libcount;
	libraries = malloc(0);
	libcount = liblist(libraries);

	void **handlers = malloc(libcount * sizeof(void*));
	libopen(libraries, handlers, libcount);

	char **function_names = malloc(0);
	int funcount;
	void (**functions)(struct complex*, float, float) = malloc(0);
	funcount = funclist(libraries, handlers, function_names, functions, libcount);

	
	printf("Input format: \"Operation Real Imaginary\", 0 to exit.\nExamples: \"+ 2 2\" or \"* 3 4\".\nAvailable operations:\n");
	for(i = 0; i < funcount; i++)
	{
		printf("%d. %s\n", i, function_names[i]);
	}

	while(1)
	{
		printf("%f+%fi\n", ptr->re, ptr->im);
		scanf(" %d",&operation);
		if(operation==0) break;
		scanf(" %f %f",&re,&im);
		functions[operation](ptr, re, im);
	}
	return 0;
}