#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

int sockcount = 1;
int *sfd;
pthread_t *tid;
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

void *funccon(void *voidclientsockaddr)
{
	char *msg = malloc(256);
	struct sockaddr *ptr = voidclientsockaddr;
	struct sockaddr clientsockaddr = *ptr;
	int socknum = sockcount-1;
	socklen_t addrlen = sizeof(clientsockaddr);
	recvfrom(sfd[socknum], msg, 256, 0, &clientsockaddr, &addrlen);
	while(strcmp(msg, "QUIT") != 0)
	{
		printf("RECIEVED ON %d: %s\n", socknum, msg);
		sprintf(msg, "%d", (int)strlen(msg));
		sendto(sfd[socknum], msg, 256, 0, &clientsockaddr, addrlen);
		recvfrom(sfd[socknum], msg, 256, 0, &clientsockaddr, &addrlen);
	}
	close(sfd[socknum]);
	free(msg);
	printf("DISCONNECTED: %d\n", socknum);
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
	int threadcount = 0;
	tid = malloc(0);
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

		threadcount++;
		tid = realloc(tid, threadcount * sizeof(pthread_t));
		pthread_create(&tid[threadcount-1], NULL, funccon, &clientsockaddr);
	}
}