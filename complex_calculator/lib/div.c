#include "../include/calc.h"

void info(plugin *plug) {
	plug->name = "Division";
	plug->functions = "div";
	plug->description = "A division plugin.";
}

void div(struct complex *num, float re, float im) {
	float t_re = (num->re*re + num->im*im) / (re*re + im*im);
	float t_im = (num->im*re - num->re*im) / (re*re + im*im);
	num->re = t_re;
	num->im = t_im;
}