#include <stdio.h>
#include <malloc.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
void func(int sig, siginfo_t *siginfo, void *vd)
{
	printf("SIGNAL: %d\n", sig);
	printf("SIG=%ld PID=%ld USR=%ld\n", (long)siginfo->si_signo, (long)siginfo->si_pid, (long)siginfo->si_uid);
}

int main()
{
	pid_t pid;
	char *str = malloc(256);
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
		struct sigaction act;
		act.sa_mask = sigmask;
		act.sa_flags = SA_SIGINFO | SA_RESTART;
		act.sa_sigaction = &func;
		struct sigaction oldact;
		sigaction(SIGUSR1, &act, &oldact);
		while(1)
		{
			sigsuspend(&sigmask);
		}
	}
	return 0;
}