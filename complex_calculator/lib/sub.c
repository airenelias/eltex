#include "../include/calc.h"

void info(plugin *plug) {
	plug->name = "Substraction";
	plug->functions = "sub&";
	plug->description = "A substraction plugin.";
}

void sub(struct complex *num, float re, float im) {
	num->re = num->re-re;
	num->im = num->im-im;
}