#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define QS_MSGLEN 256

int sfd;
char *buf;

void finish(int sig)
{
	close(sfd);
	free(buf);
	exit(0);
}

int main()
{
	sigset_t sigmask;
	sigfillset(&sigmask);
	struct sigaction act;
	act.sa_mask = sigmask;
	act.sa_flags = SA_RESTART;
	act.sa_handler = &finish;
	sigaction(SIGINT, &act, NULL);

	struct sockaddr_in sockaddr;
	buf = malloc(QS_MSGLEN);
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int bro = 1;
	setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &bro, sizeof(int));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("224.0.0.1", &sockaddr.sin_addr);

	while(1)
	{
		socklen_t addrlen = sizeof(sockaddr);
		scanf(" %[^\n]s", buf);
		sendto(sfd, buf, QS_MSGLEN, 0, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr));
	}
}