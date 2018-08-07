#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>

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

	buf = malloc(QS_MSGLEN);
	struct sockaddr_in sockaddr;
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("10.0.2.255", &sockaddr.sin_addr);
	bind(sfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	while(1)
	{
		int addrlen = sizeof(sockaddr);
		recvfrom(sfd, buf, QS_MSGLEN, 0, (struct sockaddr*)&sockaddr, &addrlen);
		printf("%s\n", buf);
	}
}