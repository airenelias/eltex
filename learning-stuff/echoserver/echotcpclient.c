#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

int main()
{
	char *buf = malloc(256);
	struct sockaddr_in sockaddr;
	int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	connect(sfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	scanf("%[^\n]s", buf);
	send(sfd, buf, 256, 0);
	recv(sfd, buf, 256, 0);
	printf("RECIEVED: %s\n", buf);
	close(sfd);
	return 0;
}