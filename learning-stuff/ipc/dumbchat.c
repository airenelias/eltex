#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

struct msgbuf {
	long int type;
	int id;
	char str[256];
};

int main()
{
	int usercount = 0;
	int i, j, userexist;
	char **users = malloc(0);
	key_t key = ftok("hello.c", 'A');
	int msgid = msgget(key, IPC_CREAT | 00666);
	size_t msgsize = 0;
	struct msgbuf msg;
	while(1)
	{
		msgsize = msgrcv(msgid, &msg, sizeof(msg), -2L, 0);
		if(msgsize != 0)
		{
			if(msg.type == 2L)
			{
				if(usercount > 0)
				{
					for(i=0; i<usercount; i++)
					{
						if(strcmp(users[i], msg.str) == 0)
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
					for(j=i; j<usercount-1; j++)
						users[j] = users[j+1];
					usercount--;
				}
				else
				{
					usercount++;
					users = realloc(users, usercount*sizeof(char**));
					users[usercount-1] = malloc(256);
					strncpy(users[usercount-1], msg.str, 256);
					msg.type = 4L;
					msg.id = usercount - 1;
					msgsnd(msgid, &msg, sizeof(msg), 0);
				}

				for(i=0; i<usercount; i++)
				{
					printf("%s\n", users[i]);
				}
			}
			if(msg.type == 1L)
			{
				char message[256];
				strncpy(message, msg.str, 256);
				char nickname[256];
				msg.type = 3L;
				for(i=0; i<usercount; i++)
				{
					strncpy(nickname, users[msg.id], 256);
					strcat(nickname, ": ");
					strcat(nickname, message);
					strncpy(msg.str, nickname, 256);
					msgsnd(msgid, &msg, sizeof(msg), 0);
				}
			}
		}
	}
	return 0;
}