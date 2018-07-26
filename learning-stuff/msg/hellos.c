#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <malloc.h>

struct msgbuf {
	long type;
	char *str;
};

int main()
{
	int pid;
	key_t key = ftok("hello.c", 'A');
	int msgid = msgget(key, IPC_CREAT | 00777);
	if(pid = fork() == 0)
	{
		char *buf = "Hello im child!";
		struct msgbuf msg;
		msg.type = 1L;
		msg.str = malloc(15);
		msg.str = buf;
		msgsnd(msgid, &msg, sizeof(msg), 0);
		msgrcv(msgid, &msg, sizeof(msg), 2L, 0);
		printf("Im child, recieved: %s\n", msg.str);
	}
	else
	{
		char *buf = "Hello im parent";
		struct msgbuf msg;
		msg.type = 2L;
		msg.str = malloc(15);
		msg.str = buf;
		msgsnd(msgid, &msg, sizeof(msg), 0);
		msgrcv(msgid, &msg, sizeof(msg), 1L, 0);
		printf("Im parent recieved: %s\n", msg.str);
	}
	return 0;
}