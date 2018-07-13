#include <stdio.h>

typedef struct {
	char *name;
	char *functions;
	char *description;
} plugin;

struct complex {
	float re;
	float im;
};

int liblist (char***);
int libopen (char**, void***, int);
int funclist(char**, void**, char***, void (***)(struct complex*, float, float), int);
void pluginfo(void*, plugin*);
void add(struct complex*, float, float);
void sub(struct complex*, float, float);
void mul(struct complex*, float, float);
void div(struct complex*, float, float);