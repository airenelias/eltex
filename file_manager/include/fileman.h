#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#ifndef FILEMAN_H
#define FILEMAN_H
typedef struct {
	char *dir_left;
	char *dir_right;
} fileman_config;

typedef struct {
	WINDOW *head;
	WINDOW *list;
	char *dir;
	struct dirent **namelist;
	int n;
	int cur;
	int outbound;
} fileman_win;

typedef struct {
	fileman_win left;
	fileman_win right;
	int active_win;
} fileman;

/*typedef struct {
	WINDOW *head_left;
	WINDOW *head_right;
	WINDOW *win_left;
	WINDOW *win_right;
	char *dir_left;
	char *dir_right;
	struct dirent **namelist_left;
	struct dirent **namelist_right;
	int n_left;
	int n_right;
	int cur_left;
	int cur_right;
	int active_win;
} fileman;*/

void startfileman(char*, char*);
fileman_config initfilemanparams(char*, char*);
fileman initfileman(fileman_config);
void inputdir(fileman_win*);
void mainfileman(fileman*);
void printdir(fileman_win*, char*, int);
void deselectfileman(fileman_win*);
void selectfileman(fileman_win*);
#endif