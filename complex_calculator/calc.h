#include <stdio.h>

struct complex {
	float re;
	float im;
};

void add(struct complex*, float, float);
void sub(struct complex*, float, float);
void mul(struct complex*, float, float);
void div(struct complex*, float, float);