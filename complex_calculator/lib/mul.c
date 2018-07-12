#include "../include/calc.h"

void info(plugin *plug) {
	plug->name = "Multiplication";
	plug->functions = "mul&";
	plug->description = "A multiplication plugin.";
}

void mul(struct complex *num, float re, float im) {
	float t_re = num->re*re - num->im*im;
	float t_im = num->im*re + num->re*im;
	num->re = t_re;
	num->im = t_im;
}