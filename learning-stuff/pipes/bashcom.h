#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fileman.h"

#ifndef BASHCOM_H
#define BASHCOM_H
///Set maximum command count.
#define BUF_MAX_COM 32

/**Execute input command.
\param char* Command string.
\warning Default path is /bin/.
*/
int bashcommand(char*);
#endif