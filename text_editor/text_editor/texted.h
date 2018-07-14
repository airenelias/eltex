#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *name;
	char *text;
	int cury;
	int curx;
} texted_config;

typedef struct {
	WINDOW *head;
	WINDOW *textfield;
	int lines;
} texted;

void starttexted(char*, char*, int, int);
texted_config inittextedparams(char*, char*, int, int);
void openfile(texted*, char*);
texted inittexted(texted_config);
char* edithead(texted*);
void edittexted(texted*);
void opentexted(texted*);
void savetexted(texted*);