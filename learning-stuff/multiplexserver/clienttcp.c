#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>

int *sfd;
char *buf;
struct sockaddr_in sockaddr;

void finish(int sig)
{
	socklen_t addrlen = sizeof(sockaddr);
	sprintf(buf, "%s", "QUIT");
	sendto(*sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, addrlen);
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

	int i=0;
	sfd = malloc(sizeof(int));
	buf = malloc(256);
	*sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3111);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);

	printf("INPUT NAME: ");
	scanf(" %[^\n]s", buf);
	socklen_t addrlen = sizeof(sockaddr);
	connect(*sfd, (struct sockaddr*)&sockaddr, addrlen);
	send(*sfd, buf, 256, 0);
	recv(*sfd, buf, 256, 0);
	printf("%s\n", buf);
	while(1)
	{
		scanf(" %[^\n]s", buf);
		send(*sfd, buf, 256, 0);
		recv(*sfd, buf, 256, 0);
		printf("%s\n", buf);
	}
}