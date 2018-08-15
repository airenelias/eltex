#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#define UDP_RESERVE 2048

int *sfd;
char *buf;

void finish(int sig)
{
	close(*sfd);
	free(sfd);
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

	sfd = malloc(sizeof(int));
	buf = malloc(MAX_STR_LEN);
	struct sockaddr_in sockaddr;
	*sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(*sfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	while(1)
	{
		memset(buf, 0, MAX_STR_LEN);
		socklen_t addrlen = sizeof(sockaddr);
		recvfrom(*sfd, buf, UDP_RESERVE, 0, (struct sockaddr*)&sockaddr, &addrlen);
		printf("RECIEVED: %s\n", buf);
		sprintf(buf, "%d", (int)strlen(buf));
		sendto(*sfd, buf, (int)strlen(buf), 0, (struct sockaddr*)&sockaddr, addrlen);
	}
}