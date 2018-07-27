#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>

struct msgpack {
	int type;
	int id;
	char str[256];
};

int main()
{
	pid_t pid;
	char str[256];
	char nick[256];

	key_t key = ftok("shmchat.c", 'A');
	key_t keyshm = ftok("shmchat.c", 'B');
	int sem_id = semget(key, 2, 0);
	struct sembuf serlock = {0, 2, 0};
	struct sembuf lock[2] = {0, 0, 0, 0, 1, 0};
	struct sembuf pass = {0, 0, 0};
	struct sembuf readlock = {1, -1, 0};
	int shm_id = shmget(keyshm, sizeof(struct msgpack), 0);
	size_t msgsize = 0;
	struct msgpack *msg = shmat(shm_id, NULL, 0);
	int myid = 0;

	printf("Nickname: ");
	scanf(" %s", nick);

	semop(sem_id, lock, 2);
	strcpy(msg->str, nick);
	msg->type = 2;

	semop(sem_id, &pass, 1);
	printf("%s, you're connected under %d\n", msg->str, msg->id);
	myid = msg->id; 
	if(pid = fork() == 0)
		while(1)
		{
			semop(sem_id, &readlock, 1);
			printf("%s\n", msg->str);
			sleep(1);
		}
	else
	{
		while(1)
		{
			scanf(" %[^\n]s", str);
			if(strcmp(str, "q") == 0)
				break;

			semop(sem_id, lock, 2);
			msg->id = myid;
			strcpy(msg->str, str);
			msg->type = 1;
			semop(sem_id, &pass, 1);
		}
		semop(sem_id, lock, 2);
		msg->id = myid;
		strcpy(msg->str, nick);
		msg->type = 2;
		kill(pid, SIGKILL);
	}
	return 0;
}