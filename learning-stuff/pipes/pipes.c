#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int fd[2];
	pid_t pid;
	pipe(fd);
	if(pid = fork() == 0)
	{
		char *strout = malloc(10);
		close(fd[1]);
		read(fd[0], strout, 10);
		printf("%s\n", strout);
		free(strout);
		return 0;
	}
	else
	{
		char *strin = malloc(10);
		close(fd[0]);
		strin = "String lol";
		write(fd[1], strin, 10);
		waitpid(pid, NULL, 0);
		return 0;
	}
}