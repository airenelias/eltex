#include "../include/calc.h"
#include <dlfcn.h>
#include <malloc.h>
#include <string.h>

int liblist(char **libraries)
{
	printf("Input paths to libraries one by one.\n");
	int i = 0;
	int size = 1;
	char continue_answer;
	char *t_path = malloc(30 * sizeof(char));
	while(1)
	{
		scanf("%s", t_path);
		int path_len = strlen(t_path);
		char *library = realloc(libraries, size * sizeof(char**));
		library = malloc(path_len * sizeof(char));
		for(i = 0; i < path_len; i++)
			*(library + i) = *(t_path + i);
		libraries[size - 1] = library;

		//char **library = realloc(libraries, size * sizeof(char**));
		//*library = malloc(path_len * sizeof(char));
		//for(i = 0; i < path_len; i++)
		//	*(library + i) = *(t_path + i);
		//libraries[size - 1] = library;

		printf("Continue? [y/n]\n");
		scanf(" %c", &continue_answer);
		if(continue_answer == 'y')
			size++;
		else
			break;
	}
	free(t_path);
	return size;
}

int libopen(char **libraries, void **handlers, int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		handlers[i] = dlopen(libraries[i], RTLD_NOW);
	}
	return 0;
}

int funclist(char **libraries, void **handlers, char **function_names, void (**functions)(struct complex*,float,float), int count)
{
	int i = 0, j = 0, funcount = 0;
	char *function_name;
	plugin *plug = malloc(sizeof(plugin));
	for(i = 0; i < count; i++)
	{
		pluginfo(handlers[i], plug);
		function_name = strtok(plug->functions,"&");
		while(function_name != NULL)
		{
			funcount++;
			char *func_name_ptr = realloc(function_names, funcount * sizeof(char**));
			func_name_ptr = malloc(10*sizeof(char*));
			*func_name_ptr = *function_name;
			function_names[funcount-1]=func_name_ptr;
			void (**function_pointer)(struct complex*, float, float) = realloc(functions, funcount * sizeof(void**));
			//function_pointer = malloc(sizeof(void*));
			*function_pointer = dlsym(handlers[i],function_name);
			function_name = strtok(NULL,"&");
		}
	}
	return funcount;
}

void pluginfo(void *dd, plugin *plug)
{
	void (*p)(plugin*);
	p = dlsym(dd,"info");
	p(plug);
}

