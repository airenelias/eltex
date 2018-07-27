#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>

struct msgbuf {
	long int type;
	int id;
	char str[256];
};

int main()
{
	pid_t pid;
	char str[256];
	char nick[256];
	key_t key = ftok("hello.c", 'A');
	int msgid = msgget(key, 0);
	int myid = 0;
	struct msgbuf msg;

	printf("Nickname: ");
	scanf(" %s", nick);
	msg.type = 2L;
	msg.id = 0;
	strcpy(msg.str, nick);
	msgsnd(msgid, &msg, sizeof(msg), 0);
	msgrcv(msgid, &msg, sizeof(msg), 4L, 0);
	printf("%s, you're connected under %d\n", msg.str, msg.id);
	myid = msg.id; 
	if(pid = fork() == 0)
		while(1)
		{
			msgrcv(msgid, &msg, sizeof(msg), 3L, 0);
			printf("%s\n", msg.str);
			sleep(1);
		}
	else
	{
		while(1)
		{
			scanf(" %[^\n]s", str);
			if(strcmp(str, "q") == 0)
				break;
			msg.type = 1L;
			msg.id = myid;
			strcpy(msg.str, str);
			msgsnd(msgid, &msg, sizeof(msg), 0);
		}
		msg.type = 2L;
		strcpy(msg.str, nick);
		msgsnd(msgid, &msg, sizeof(msg), 0);
		kill(pid, SIGKILL);
	}
	return 0;
}