#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
void func(int sig)
{
	printf("SIGNAL: %d\n", sig);
}

int main()
{
	pid_t pid;
	if(pid = fork() == 0)
	{
		while(1)
		{
			getchar();
			kill(getppid(), SIGUSR1);
		}
	}
	else
	{	
		sigset_t sigmask, oldsigmask;
		sigemptyset(&sigmask);
		signal(SIGUSR1, func);
		while(1)
		{
			sigsuspend(&sigmask);
		}
	}
	return 0;
}