#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	int status;
	int pid1 = fork();
	if(pid1 == 0)
	{
		printf("2 my=%d pid=%d ppid=%d\n",pid1, getpid(), getppid());
		return 0;
	}
	else
	{
		printf("Pochemy 2 raza");
		int pid2 = fork();
		if(pid2 == 0)
		{
			printf("3 my=%d pid=%d ppid=%d\n",pid2, getpid(), getppid());
			return 0;
		}
		else
		{
			printf("1 my=%d pid=%d ppid=%d\n",pid2, getpid(), getppid());
			waitpid(pid1, &status, 0);
			waitpid(pid2, &status, 0);
		}
	}
	return 0;
}