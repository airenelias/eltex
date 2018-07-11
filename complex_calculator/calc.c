#include "calc.h"

void add(struct complex *num, float re, float im) {
	num->re = num->re+re;
	num->im = num->im+im;
}
void sub(struct complex *num, float re, float im) {
	num->re = num->re-re;
	num->im = num->im-im;
}
void mul(struct complex *num, float re, float im) {
	float t_re = num->re*re - num->im*im;
	float t_im = num->im*re + num->re*im;
	num->re = t_re;
	num->im = t_im;
}
void div(struct complex *num, float re, float im) {
	float t_re = (num->re*re + num->im*im) / (re*re + im*im);
	float t_im = (num->im*re - num->re*im) / (re*re + im*im);
	num->re = t_re;
	num->im = t_im;
}