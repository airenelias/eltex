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
	buf = malloc(256);
	struct sockaddr_in sockaddr;
	*sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(*sfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	listen(*sfd, 1);
	while(1)
	{
		socklen_t addrlen = sizeof(sockaddr);
		int cfd = accept(*sfd, (struct sockaddr*)&sockaddr, &addrlen);
		recv(cfd, buf, 256, 0);
		printf("RECIEVED: %s\n", buf);
		sprintf(buf, "%d", (int)strlen(buf));
		send(cfd, buf, 256, 0);
		close(cfd);
	}
}