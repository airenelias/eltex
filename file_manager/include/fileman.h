/** \mainpage FileMan
Most unused ncurses file manager.
*/

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#ifndef FILEMAN_H
#define FILEMAN_H
///Struct that stores manager configurations.
typedef struct {
	char *dir_left; ///<	Left window filepath.
	char *dir_right; ///<	Right window filepath.
} fileman_config;

///Struct that stores parameters one of two windows in manager.
typedef struct {
	WINDOW *head; ///< 				Window displaying path.
	WINDOW *list; ///< 				Window displaying list of files.
	char *dir; ///<					Current window directory.
	struct dirent **namelist; ///<	List of files.
	int n; ///<						Number of files.
	int cur; ///<					Current selection position.
	int outbound; ///<				How much out of window bounds is selection.\warning Must be 0 or more. That means only downwards list movement allowed.
} fileman_win;

///Struct that stores parameters manager.
typedef struct {
	fileman_win left; ///<	Left manager window.
	fileman_win right; ///<	Right manager window.
	int active_win; ///<	Currently active window.\warning Must be 0 or 1 for left or right window respectively.
} fileman;

/**Initialize configuration, manager window and start controls.
\param char* Left window filepath.
\param char* Right window filepath.
\warning Input paths is relative to current launch directory if not absolute.
*/
void startfileman(char*, char*);

/**Initialize configuration struct.
\param char* Left window filepath.
\param char* Right window filepath.
\warning Input paths is relative to current launch directory if not absolute.
*/
fileman_config initfilemanparams(char*, char*);

/**Initialize manager window.
\param fileman_config Configuration struct.
*/
fileman initfileman(fileman_config);

/**Trasfer input to header of selected window for path input.
\param fileman_win* Currently working window.
*/
void inputdir(fileman_win*);

/**Main controls function.
\param fileman* Manager to work with.
*/
void mainfileman(fileman*);

/**Print all files in directory.
\param fileman_win* Print directory window.
\param char* 		Path to print.\warning Input path is relative to current launch directory if not absolute.
\param int 			Reinitialize window parameters.\warning Must be 1 if directory changes.
*/
void printdir(fileman_win*, char*, int);

/**Remove highlight of menu element.
\param fileman_win* Currently working window.
*/
void deselectfileman(fileman_win*);

/**Highlights menu element.
\param fileman_win* Currently working window.
*/
void selectfileman(fileman_win*);
#endif