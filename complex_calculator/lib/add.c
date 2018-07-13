#include "../include/calc.h"

void info(plugin *plug) {
	plug->name = "Addition";
	plug->functions = "add";
	plug->description = "An addition plugin.";
}

void add(struct complex *num, float re, float im) {
	num->re = num->re+re;
	num->im = num->im+im;
}