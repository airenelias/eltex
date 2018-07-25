#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	pid_t pid;
	mkfifo("pipe", S_IRWXU);
	if(pid = fork() == 0)
	{

		char *strout = malloc(10);
		int file = open("pipe", O_RDONLY);
		read(file, strout, 10);
		printf("%s\n", strout);
		free(strout);
		return 0;
	}
	else
	{
		char *strin = malloc(10);
		strin = "String lol";
		int file = open("pipe", O_WRONLY);
		write(file, strin, 10);
		waitpid(pid, NULL, 0);
		unlink("pipe");
		return 0;
	}
}