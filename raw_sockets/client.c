#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#define MAX_STR_LEN 1024
#define UDP_RESERVE 2048

#define IP_SRC "192.168.1.24"
#define IP_DST "192.168.1.25"
#define PORT_SRC 3111
#define PORT_DST 3110

struct udp_header {
	unsigned short int port_src;
	unsigned short int port_dst;
	unsigned short int len;
	unsigned short int csum;
};

struct ip_header {
	unsigned int ihl:4;
	unsigned int version:4;
	unsigned int dscp:6;
	unsigned int ecn:2;
	unsigned short int total_len;
	unsigned short int identification;
	unsigned int flags:3;
	unsigned int offset:13;
	unsigned int ttl:8;
	unsigned int protocol:8;
	unsigned short int csum;
	unsigned int ip_src;
	unsigned int ip_dst;
};

struct mac_header {
	unsigned char mac_dst[ETH_ALEN];
	unsigned char mac_src[ETH_ALEN];
	unsigned short int type;
};

unsigned short int csum_calc(unsigned short int *addr, unsigned int count) {
	unsigned long int sum = 0;
	while(count > 1) {
		sum += *addr++;
		count -= 2;
	}
	if(count > 0) {
		sum += ((*addr) & htons(0xFF00));
	}
	while (sum >> 16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}
	sum = ~sum;
	return ((unsigned short int) sum);
}

char *get_ip(int ip)
{
	char *ip_str = malloc(16);
	sprintf(ip_str, "%d.%d.%d.%d", ip>>24 & 0xFF, ip>>16 & 0xFF, ip>>8 & 0xFF, ip & 0xFF);
	return ip_str;
}

int print_packet(void *rcv_packet)
{
	int i;
	struct mac_header *packet_mac_head = rcv_packet;
	struct ip_header *packet_ip_head = rcv_packet + sizeof(struct mac_header);
	struct udp_header *packet_udp_head = rcv_packet + sizeof(struct mac_header) + sizeof(struct ip_header);
	if(ntohs(packet_udp_head->port_dst) == PORT_SRC) {
		printf("ETH Header:\n");
		printf(" Destination MAC: ");
		for(i=0; i<ETH_ALEN; i++) printf("%d:", packet_mac_head->mac_dst[i]);
		printf("\n Source MAC: ");
		for(i=0; i<ETH_ALEN; i++) printf("%d:", packet_mac_head->mac_src[i]);
		printf("\n Type: %d\n", packet_mac_head->type);

		printf("IPv4 Header:\n");
		printf(" Version: %d\n", packet_ip_head->version);
		printf(" IHL: %d\n", packet_ip_head->ihl);
		printf(" DSCP: %d\n", packet_ip_head->dscp);
		printf(" ECN: %d\n", packet_ip_head->ecn);
		printf(" Total Length: %d\n", ntohs(packet_ip_head->total_len));
		printf(" Identification: %d\n", ntohs(packet_ip_head->identification));
		printf(" Flags: %d\n", packet_ip_head->flags);
		printf(" Offset: %d\n", packet_ip_head->offset);
		printf(" TTL: %d\n", packet_ip_head->ttl);
		printf(" Protocol: %d\n", packet_ip_head->protocol);
		printf(" Checksum: %d\n", packet_ip_head->csum);
		char *ip_str = get_ip(ntohl(packet_ip_head->ip_src));
		printf(" Source IP: %s\n", ip_str);
		free(ip_str);
		ip_str = get_ip(ntohl(packet_ip_head->ip_dst));
		printf(" Destination IP: %s\n", ip_str);
		free(ip_str);

		printf("UDP Header:\n");
		printf(" Source port: %d\n", ntohs(packet_udp_head->port_src));
		printf(" Destination port: %d\n", ntohs(packet_udp_head->port_dst));
		printf(" Length: %d\n", ntohs(packet_udp_head->len));
		printf(" Checksum: %d\n", packet_udp_head->csum);

		char *data = rcv_packet + sizeof(struct mac_header) + sizeof(struct ip_header) + sizeof(struct udp_header);
		printf("Data:\n%.*s\n", (int)(ntohs(packet_udp_head->len) - sizeof(struct udp_header)), data);
		return 0;
	}
	else
		return -1;
}

int main()
{
	char *buf = malloc(MAX_STR_LEN);
	int flag = 1;
	struct sockaddr_ll sockaddr;
	int sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sfd == -1) {
		perror("Can't create raw socket");
		return -1;
	}
	char mac_str_dst[ETH_ALEN] = {0x08, 0x00, 0x27, 0x00, 0x00, 0x01};
	char mac_str_src[ETH_ALEN] = {0x08, 0x00, 0x27, 0x00, 0x00, 0x00};

	sockaddr.sll_family = AF_PACKET;
	sockaddr.sll_protocol = htons(ETH_P_IP);
	sockaddr.sll_ifindex = if_nametoindex("enp0s8");
	sockaddr.sll_halen = ETH_ALEN;
	memcpy(sockaddr.sll_addr, mac_str_src, ETH_ALEN);
	socklen_t addrlen = sizeof(sockaddr);
	//get message
	scanf("%[^\n]s", buf);
	int packet_len = strlen(buf);
	//eth header
	packet_len += sizeof(struct mac_header);
	struct mac_header machead;
	memcpy(machead.mac_dst, mac_str_dst, ETH_ALEN);
	memcpy(machead.mac_src, mac_str_src, ETH_ALEN);
	machead.type = htons(ETH_P_IP);
	//udp header
	packet_len += sizeof(struct udp_header);
	struct udp_header udphead;
	udphead.port_src = htons(PORT_SRC);
	udphead.port_dst = htons(PORT_DST);
	udphead.csum = 0;
	udphead.len = htons(packet_len - sizeof(struct mac_header));
	//ip header
	packet_len += sizeof(struct ip_header);
	struct ip_header iphead;
	iphead.ihl = 5;
	iphead.version = 4;
	iphead.dscp = 0;
	iphead.ecn = 0;
	iphead.total_len = htons(packet_len - sizeof(struct mac_header));
	iphead.identification = htons(1234);
	iphead.flags = 0;
	iphead.offset = 0;
	iphead.ttl = 255;
	iphead.protocol = IPPROTO_UDP;
	iphead.csum = 0;
	iphead.ip_src = inet_addr(IP_SRC);
	iphead.ip_dst = inet_addr(IP_DST);
	iphead.csum = csum_calc((unsigned short int*)&iphead, sizeof(struct ip_header));
	//concat packet
	void *snd_packet = malloc(packet_len);
	void *rcv_packet = malloc(UDP_RESERVE);
	memcpy(snd_packet, &machead, sizeof(machead));
	memcpy(snd_packet + sizeof(machead), &iphead, sizeof(iphead));
	memcpy(snd_packet + sizeof(machead) + sizeof(iphead), &udphead, sizeof(udphead));
	memcpy(snd_packet + sizeof(machead) + sizeof(iphead) + sizeof(udphead), buf, strlen(buf));
	//send
	sendto(sfd, snd_packet, packet_len, 0, (struct sockaddr*)&sockaddr, addrlen);
	do {
		recvfrom(sfd, rcv_packet, UDP_RESERVE, 0, (struct sockaddr*)&sockaddr, &addrlen);
	}
	while(print_packet(rcv_packet) != 0);
	//close
	close(sfd);
	free(buf);
	free(snd_packet);
	free(rcv_packet);
	return 0;
}