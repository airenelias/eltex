/** \mainpage FileMan
Most unused ncurses file manager.
*/

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <panel.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#ifndef FILEMAN_H
#define FILEMAN_H
///Set maximum path length.
#define BUF_PATH_SIZE 4096
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
	int n; ///<					Number of files.
	int cur; ///<					Current selection position.
	int outbound; ///<				How much out of window bounds is selection.\warning Must be 0 or more. That means only downwards list movement allowed.
} fileman_win;

///Struct that stores parameters manager.
typedef struct {
	WINDOW *status; ///<	Status window.
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
\return File manager configuration struct.
*/
fileman_config initfilemanparams(char*, char*);

/**Initialize manager window.
\param fileman_config Configuration struct.
\return fileman File manager.
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

/**Free used memory and close.
\param fileman_win* 	Manager to close.
*/
void freefileman(fileman*);

/**Set window path and reinitialize window variables.
\param fileman_win* 	Set directory window.
\param char* 		Path to set.\warning Input path is relative to current launch directory if not absolute.
*/
void setdir(fileman_win*, char*);

/**Print all files in directory set in window.
\param fileman_win* 	Print directory window.
*/
void printdir(fileman_win*);

/**Remove highlight of menu element.
\param fileman_win* Currently working window.
*/
void deselectfileman(fileman_win*);

/**Highlight menu element.
\param fileman_win* Currently working window.
*/
void selectfileman(fileman_win*);
#endif
