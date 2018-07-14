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
	libcount = liblist(&libraries);

	void **handlers;
	libopen(libraries, &handlers, libcount);

	int funcount;
	char **function_names;
	void (**functions)(struct complex*, float, float);
	funcount = funclist(libraries, handlers, &function_names, &functions, libcount);
	
	printf("\nInput format: \"<Operation> <Real> <Imaginary>\", -1 to exit.\nFor example: \"0 2 2\" or \"1 3 4\".\n Available operations:\n");
	for(i = 0; i < funcount; i++)
	{
		printf(" %d. %s\n", i, function_names[i]);
	}

	while(1)
	{
		printf("%f+%fi\n", ptr->re, ptr->im);
		scanf(" %d",&operation);
		if(operation==-1) break;
		scanf(" %f %f",&re,&im);
		functions[operation](ptr, re, im);
	}
	
	free(functions);
	free(function_names);
	for(i=0;i<libcount;i++)
	{
		dlclose(handlers[i]);
		free(libraries[i]);
	}
	free(handlers);
	free(libraries);
	return 0;
}