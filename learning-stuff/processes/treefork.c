#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid_2, pid_3, pid_4, pid_5, pid_6;
	if(pid_2 = fork() == 0)
	{
		if(pid_6 = fork() == 0)
		{
			printf("6 pid=%d ppid=%d\n", getpid(), getppid());
			return 0;
		}
		else
		{
			printf("2 pid=%d ppid=%d\n", getpid(), getppid());
			waitpid(pid_6, NULL, 0);
			return 0;
		}
		
	}
	else
	{
		if(pid_3 = fork() == 0)
		{
			if(pid_4 = fork() == 0)
			{
				printf("4 pid=%d ppid=%d\n", getpid(), getppid());
				return 0;
			}
			else
			{
				if(pid_5 = fork() == 0)
				{
					printf("5 pid=%d ppid=%d\n", getpid(), getppid());
					return 0;
				}
				else
				{
					printf("3 pid=%d ppid=%d\n", getpid(), getppid());
					waitpid(pid_4, NULL, 0);
					waitpid(pid_5, NULL, 0);
					return 0;
				}
			}
		}
		else
		{
			printf("1 pid=%d ppid=%d\n", getpid(), getppid());
			waitpid(pid_2, NULL, 0);
			waitpid(pid_3, NULL, 0);
			return 0;
		}
	}
}