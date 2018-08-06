#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>

int sockcount = 2;
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

void *funcconudp(void *voidclientsockaddr)
{
	char *msg = malloc(256);
	struct sockaddr *ptr = voidclientsockaddr;
	struct sockaddr clientsockaddr = *ptr;
	int socknum = sockcount-1;
	socklen_t addrlen = sizeof(clientsockaddr);
	recvfrom(sfd[socknum], msg, 256, 0, &clientsockaddr, &addrlen);
	while(strcmp(msg, "QUIT") != 0)
	{
		printf("RECIEVED UDP ON %d: %s\n", socknum, msg);
		sprintf(msg, "%d", (int)strlen(msg));
		sendto(sfd[socknum], msg, 256, 0, &clientsockaddr, addrlen);
		recvfrom(sfd[socknum], msg, 256, 0, &clientsockaddr, &addrlen);
	}
	close(sfd[socknum]);
	free(msg);
	printf("DISCONNECTED: %d\n", socknum);
}

void *funccontcp(void *voidclientsockaddr)
{
	char *msg = malloc(256);
	struct sockaddr *ptr = voidclientsockaddr;
	struct sockaddr clientsockaddr = *ptr;
	int socknum = sockcount-1;
	recv(sfd[socknum], msg, 256, 0);
	while(strcmp(msg, "QUIT") != 0)
	{
		printf("RECIEVED TCP ON %d: %s\n", socknum, msg);
		sprintf(msg, "%d", (int)strlen(msg));
		send(sfd[socknum], msg, 256, 0);
		recv(sfd[socknum], msg, 256, 0);
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
	int i;
	tid = malloc(0);
	sfd = malloc(sockcount * sizeof(int));
	buf = malloc(256);
	sfd[0] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(sfd[0], (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	sfd[1] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3111);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(sfd[1], (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	listen(sfd[1], 5);

	struct pollfd pollfds[2];
	pollfds[0].fd = sfd[0];
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	pollfds[1].fd = sfd[1];
	pollfds[1].events = POLLIN;
	pollfds[1].revents = 0;
	while(1)
	{
		poll(pollfds, 2 , 0);
		if(pollfds[0].revents == POLLIN)
		{
			socklen_t addrlen = sizeof(clientsockaddr);
			recvfrom(sfd[0], buf, 256, 0, (struct sockaddr*)&clientsockaddr, &addrlen);
			printf("UDP CONNECTED: %s\n", buf);
			sockcount++;
			sfd = realloc(sfd, sockcount * sizeof(int));
			sfd[sockcount-1] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_port = 0;
			inet_aton("127.0.0.1", &sockaddr.sin_addr);
			bind(sfd[sockcount-1], (struct sockaddr*)&sockaddr, sizeof(sockaddr));

			sprintf(buf, "%s", "Welcome!");
			sendto(sfd[sockcount-1], buf, 256, 0, (struct sockaddr*)&clientsockaddr, addrlen);

			threadcount++;
			tid = realloc(tid, threadcount * sizeof(pthread_t));
			pthread_create(&tid[threadcount-1], NULL, funcconudp, &clientsockaddr);
		}
		if(pollfds[1].revents == POLLIN)
		{
			socklen_t addrlen = sizeof(clientsockaddr);
			sockcount++;
			sfd[sockcount-1] = accept(sfd[1], (struct sockaddr*)&clientsockaddr, &addrlen);
			recv(sfd[sockcount-1], buf, 256, 0);
			printf("TCP CONNECTED: %s\n", buf);
			sprintf(buf, "%s", "Welcome!");
			send(sfd[sockcount-1], buf, 256, 0);

			threadcount++;
			tid = realloc(tid, threadcount * sizeof(pthread_t));
			pthread_create(&tid[threadcount-1], NULL, funccontcp, &clientsockaddr);
		}
	}
}