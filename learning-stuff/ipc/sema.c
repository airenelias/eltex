#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	key_t key = ftok("shmchat.c", 'A');
	int sem_id = semget(key, 2, IPC_CREAT | 00666);
	//struct sembuf one = {1,0,0};
	struct sembuf two = {1,-1,0};
	int i=0;
	//semop(sem_id,&one,1);
	pid_t pid;
	if(pid=fork()==0)
	while(1)
	{
		i++;
		printf("i=%d ",i);
		fflush(stdout);
		semop(sem_id,&two,1);
	}
	else
		waitpid(pid,NULL,0);
	return 0;
}