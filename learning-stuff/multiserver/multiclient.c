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
	while(i!=2000)
	{
		fflush(stdout);
		//getchar();
		i++;
		pid_t pid;
		if(pid=fork()==0)
		{
			sfd = malloc(sizeof(int));
			buf = malloc(256);
			*sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_port = htons(3110);
			inet_aton("127.0.0.1", &sockaddr.sin_addr);

			printf("INPUT NAME: ");
			sprintf(buf, "%d",i);
			//scanf(" %[^\n]s", buf);
			socklen_t addrlen = sizeof(sockaddr);
			sendto(*sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, addrlen);
			recvfrom(*sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, &addrlen);
			printf("%s", buf);
			while(1)
			{
				sprintf(buf, "%d",i);
				//scanf(" %[^\n]s", buf);
				sendto(*sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, addrlen);
				recvfrom(*sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, &addrlen);
				printf("%s\n", buf);
				sleep(100);
			}
		}
	}
}