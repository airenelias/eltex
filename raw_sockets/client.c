#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define MAX_STR_LEN 1024
#define UDP_RESERVE 2048

struct udp_header {
	short int port_src;
	short int port_dst;
	short int len;
	short int csum;
};

struct pseudo_header {
	int ip_src;
	int ip_dst;
	char nulls;
	char proto;
	short int len_udp;
	short int port_src;
	short int port_dst;
	short int len;
	short int csum;
};

int main()
{
	char *buf = malloc(MAX_STR_LEN);
	struct sockaddr_in sockaddr;
	int sfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	socklen_t addrlen = sizeof(sockaddr);

	scanf("%[^\n]s", buf);

	int packet_len = sizeof(struct udp_header)+strlen(buf);

	struct udp_header udphead;
	udphead.port_src = htons(3111);
	udphead.port_dst = htons(3110);
	udphead.csum = 0;
	udphead.len = htons(packet_len);

	void *snd_packet = malloc(packet_len);
	void *rcv_packet = malloc(UDP_RESERVE);
	memcpy(snd_packet, &udphead, sizeof(udphead));
	memcpy(snd_packet + sizeof(udphead), buf, strlen(buf));

	sendto(sfd, snd_packet, packet_len, 0, (struct sockaddr*)&sockaddr, addrlen);
	while(1) {
		recvfrom(sfd, rcv_packet, 4086, 0, (struct sockaddr*)&sockaddr, &addrlen);
		struct pseudo_header *packet_pseudo_head = rcv_packet;
		struct udp_header *packet_udp_head = rcv_packet + sizeof(struct pseudo_header);
		//if(packethead->port_dst == 3111)
		{
			char *data = rcv_packet + sizeof(struct pseudo_header) + sizeof(struct udp_header);
			printf("PORT_DST: %d\n", ntohs(packet_udp_head->port_dst));
			printf("DATA: %s\n", data);
		}
	}
	close(sfd);
	return 0;
}