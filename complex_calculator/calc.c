#include <stdio.h>
struct complex {
	float re;
	float im;
};
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
int main()
{
	char operation;
	float re=0; float im=0;
	float A[2] = {0,0};
	struct complex *number = &A;
	printf("Input format: \"Operation Real Imaginary\", q to exit.\n");
	while(1)
	{
		printf("%f+%fi\n", number->re,number->im);
		scanf(" %c",&operation);
		if(operation=='q') break;
		scanf(" %f %f",&re,&im);
		switch(operation)
		{
			case '+': add(number,re,im); break;
			case '-': sub(number,re,im); break;
			case '*': mul(number,re,im); break;
			case '/': div(number,re,im); break;

			default: break;
		}
	}
	return 0;
}