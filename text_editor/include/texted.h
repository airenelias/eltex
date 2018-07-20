#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEXTED_H
#define TEXTED_H
///Set maximum string size
#define BUF_STR_SIZE 256
///Struct that stores text editor configurations.
typedef struct {
	char *name; ///<	File name.
	char *text; ///<	Append text.
	int cury; ///<		Init cursor Y position.
	int curx; ///<		Init cursor X position.
} texted_config;

///Struct that stores text editor parameters.
typedef struct {
	WINDOW *head; ///< 		Window displaying path.
	WINDOW *textfield; ///<	Window displaying text.
	int cury; ///<			Current cursor Y position.
	int curx; ///<			Current cursor X position.
	int lines; ///<			File lines count.
} texted;

/**Create text editor configuration struct, init text editor window and start controls.
\param char* File path.
\param char* Append string.
\param int Cursor Y position.
\param int Cursor X position.
*/
void starttexted(char*, char*, int, int, int);

/**Create text editor configuration struct.
\param char* File path.
\param char* Append string.
\param int Cursor Y position.
\param int Cursor X position.
*/
texted_config inittextedparams(char*, char*, int, int);

/**Open file and print its text lines to textfield.
\param texted* Text editor.
\param char* File path.
*/
void openfile(texted*, char*);

/**Initialize text editor windows and parameters.
\param texted_config Text editor configuration struct.
\return texted Text editor.
*/
texted inittexted(texted_config);

/**Transfer input to header for path input.
\param texted* Text editor.
\return char* File path.
*/
char* edithead(texted*);

/**Main controls function.
\param texted* Text editor.
\param int Finish work with ncurses flag.\warning Must be set to 0 if called from other ncurses environment.
*/
void edittexted(texted*, int);

/**Transfer input to header and open file.
\param texted* Text editor.
*/
void opentexted(texted*);

/**Transfer input to header and save textfield lines to file.
\param texted* Text editor.
*/
void savetexted(texted*);
#endif