#include "../include/calc.h"
#include <dlfcn.h>
#include <malloc.h>
#include <string.h>

int liblist(char ***libraries)
{
	printf("Input paths to libraries one by one.\n");
	int i = 0;
	int size = 1;
	char continue_answer;
	*libraries = malloc(0);
	char *t_path;
	while(1)
	{
		printf("Path: ");
		t_path = malloc(30 * sizeof(char));
		scanf("%s", t_path);
		int path_len = strlen(t_path);
		*libraries = realloc(*libraries, size * sizeof(char**));
		(*libraries)[size - 1] = malloc(path_len * sizeof(char));
		for(i=0;i<path_len;i++)
			(*libraries)[size - 1][i] = t_path[i];
		free(t_path);
		printf("Continue? [y/n] ");
		scanf(" %c", &continue_answer);
		if(continue_answer == 'y')
			size++;
		else
			break;
	}
	return size;
}

int libopen(char **libraries, void ***handlers, int count)
{
	int i;
	*handlers = malloc(count * sizeof(void*));
	for(i = 0; i < count; i++)
	{
		(*handlers)[i] = dlopen(libraries[i], RTLD_NOW);
	}
	return 0;
}

int funclist(char **libraries, void **handlers, char ***function_names, void (***functions)(struct complex*,float,float), int count)
{
	int i = 0, j = 0, funcount = 0;
	*function_names = malloc(0);
	*functions = malloc(0);
	char *function_name;
	plugin *plug = malloc(sizeof(plugin));
	for(i = 0; i < count; i++)
	{
		pluginfo(handlers[i], plug);
		function_name = strtok(plug->functions,"&");
		while(function_name != NULL)
		{
			funcount++;
			*function_names = realloc(*function_names, funcount * sizeof(char**));
			//(*function_names)[funcount - 1] = malloc(strlen(function_name) * sizeof(char*));
			(*function_names)[funcount - 1] = function_name;
			*functions = realloc(*functions, funcount * sizeof(void (**)(struct complex*,float,float)));
			//(*functions)[funcount - 1] = malloc(sizeof(void (*)(struct complex*,float,float)));
			(*functions)[funcount - 1] = dlsym(handlers[i], function_name);
			function_name = strtok(NULL,"&");
		}
	}
	free(plug);
	return funcount;
}

void pluginfo(void *dd, plugin *plug)
{
	void (*p)(plugin*);
	p = dlsym(dd,"info");
	p(plug);
}

