#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	int status;
	int pid1 = fork();
	if(pid1 == 0)
	{
		printf("\n2 my=%d pid=%d ppid=%d",pid1, getpid(), getppid());
		return 0;
	}
	else
	{
		printf("This goes to buffer");
		fflush(stdout); //flush or else child process will have same buffer as parent
		int pid2 = fork();
		if(pid2 == 0)
		{
			printf("\n3 my=%d pid=%d ppid=%d",pid2, getpid(), getppid());
			return 0;
		}
		else
		{
			//its usually first, but \n clears buffer and it becomes last :(
			printf("\n1 my=%d pid=%d ppid=%d",pid2, getpid(), getppid());
			waitpid(pid1, &status, 0);
			waitpid(pid2, &status, 0);
		}
	}
	return 0;
}