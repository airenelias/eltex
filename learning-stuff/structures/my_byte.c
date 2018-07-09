#include <stdio.h>
struct my_byte
{
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
	unsigned int b8:1;
};

struct my_str
{
	char c;
	int d;
};//__attribute__((packed));

int main()
{
	int i=0;
	char c = 15;
	struct my_byte *pb = &c;
	printf("%d%d%d%d%d%d%d%d\n", pb->b1, pb->b2, pb->b3, pb->b4, pb->b5, pb->b6, pb->b7, pb->b8);
	printf("%d\n\n", *pb);

	char str[10]={'A',0,0,0,0,'B',0,0,0,0};
	struct my_str *ps = &str;
	pb = &ps->d;
	printf("%c %d\n", ps->c, ps->d);
	for(i=0;i<4;i=i+1)
		printf("%d%d%d%d%d%d%d%d\n", (pb+i)->b1, (pb+i)->b2, (pb+i)->b3, (pb+i)->b4, (pb+i)->b5, (pb+i)->b6, (pb+i)->b7, (pb+i)->b8);
	return 0;
}