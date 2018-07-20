#include "../include/texted.h"

int main(int argc, char **argv)
{
	if(argc == 0)
		starttexted("", "", 0, 0, 1);
	else
		starttexted(argv[1], "", 0, 0, 1);
	return 0;
}
