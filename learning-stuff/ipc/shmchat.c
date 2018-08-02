#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>


struct msgpack {
	int type;
	int id;
	char str[256];
};

int main()
{
	int usercount = 0;
	int msgcount = 0;
	int i, j, userexist;
	char **users = malloc(0);
	key_t key = ftok("shmchat.c", 'A');
	key_t keyshm = ftok("shmchat.c", 'B');
	int sem_id = semget(key, 2, IPC_CREAT | 00666);
	printf("SEMAPHORE = %d\n",sem_id);
	//struct sembuf serlock = {0, 2, 0};
	struct sembuf unlock = {0, -1, 0};
	struct sembuf lock[2] = {0, 0, 0, 0, 1, 0};
	struct sembuf readlock = {1, -1, 0};

	int shm_id = shmget(keyshm, sizeof(struct msgpack), IPC_CREAT | 00666);
	size_t msgsize = 0;
	struct msgpack *msg = shmat(shm_id, NULL, 0);
	msg->type=-1;
	msg->id=-1;
	strncpy(msg->str, "", 256);

	while(1)
	{
		if(msg->type > 2 || msg->type < 1)
			continue;
		else
		{
			if(msg->type == 2)
			{
				if(usercount > 0)
				{
					for(i=0; i<usercount; i++)
					{
						if(strcmp(users[i], msg->str) == 0)
						{
							userexist = 1;
							break;
						}
						else userexist = 0;
					}
				}
				else userexist = 0;
				if(userexist)
				{
					//for(j=i; j<usercount-1; j++)
					//	users[j] = users[j+1];
					msgcount--;
					
					msg->type = 4;
				}
				else
				{
					usercount++;
					msgcount++;
					users = realloc(users, usercount*sizeof(char**));
					users[usercount-1] = malloc(256);
					strncpy(users[usercount-1], msg->str, 256);
					msg->type = 4;
					msg->id = usercount - 1;
				}

				for(i=0; i<usercount; i++)
				{
					printf("%s\n", users[i]);
				}
			}
			if(msg->type == 1)
			{
				char message[256];
				strncpy(message, msg->str, 256);
				char nickname[256];
				msg->type = 3;
				strncpy(nickname, users[msg->id], 256);
				strcat(nickname, ": ");
				strcat(nickname, message);
				strncpy(msg->str, nickname, 256);
				struct sembuf readunlock[2] = {1, 0, 0, 1, msgcount, 0};
				semop(sem_id, readunlock, 2);
			}
		}
		semop(sem_id, &unlock, 1);
	}
	return 0;
}