#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>

int sockcount = 1;
int *sfd;
char *buf;

void finish(int sig)
{
	int i;
	for(i = 0; i<sockcount; i++)
		close(sfd[i]);
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

	struct sockaddr_in sockaddr, clientsockaddr;
	sfd = malloc(sockcount * sizeof(int));
	buf = malloc(256);
	sfd[0] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(sfd[0], (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	while(1)
	{
		socklen_t addrlen = sizeof(clientsockaddr);
		recvfrom(sfd[0], buf, 256, 0, (struct sockaddr*)&clientsockaddr, &addrlen);
		printf("CONNECTED: %s\n", buf);
		sockcount++;
		sfd = realloc(sfd, sockcount * sizeof(int));
		sfd[sockcount-1] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = 0;
		inet_aton("127.0.0.1", &sockaddr.sin_addr);
		bind(sfd[sockcount-1], (struct sockaddr*)&sockaddr, sizeof(sockaddr));

		sprintf(buf, "%s", "Welcome!\n");
		sendto(sfd[sockcount-1], buf, 256, 0, (struct sockaddr*)&clientsockaddr, addrlen);

		pid_t pid;
		if(pid = fork() == 0)
		{
			char *msg = malloc(256);
			recvfrom(sfd[sockcount-1], msg, 256, 0, (struct sockaddr*)&clientsockaddr, &addrlen);
			while(strcmp(msg, "QUIT") != 0)
			{
				printf("RECIEVED ON %d: %s\n", sockcount-1, msg);
				sprintf(msg, "%d", (int)strlen(msg));
				sendto(sfd[sockcount-1], msg, 256, 0, (struct sockaddr*)&clientsockaddr, addrlen);
				recvfrom(sfd[sockcount-1], msg, 256, 0, (struct sockaddr*)&clientsockaddr, &addrlen);
			}
			free(msg);
			exit(0);
		}
	}
}