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
	int sfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	scanf("%[^\n]s", buf);
	socklen_t addrlen = sizeof(sockaddr);
	sendto(sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, addrlen);
	recvfrom(sfd, buf, 256, 0, (struct sockaddr*)&sockaddr, &addrlen);
	printf("RECIEVED: %s\n", buf);
	close(sfd);
	return 0;
}