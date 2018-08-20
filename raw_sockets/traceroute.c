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

//#define IP_SRC "192.168.1.24"
//#define IP_DST "192.168.1.1"
#define IP_SRC "192.168.1.24"
#define IP_DST "195.93.187.17"
//#define PORT_SRC 3111
//#define PORT_DST 3110

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

struct icmp_echo {
	unsigned int type:8;
	unsigned int code:8;
	unsigned short int csum;
	unsigned short int id;
	unsigned short int seq;
	unsigned int payload;
};

struct icmp_time_exceeded {
	unsigned int type:8;
	unsigned int code:8;
	unsigned short int csum;
	unsigned int unused;
	struct ip_header ip_header;
	struct icmp_echo icmp_echo;
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

int print_packet(void *rcv_packet, int id, int* counter)
{
	int i;
	struct mac_header *packet_mac_head = rcv_packet;
	struct ip_header *packet_ip_head = rcv_packet + sizeof(struct mac_header);
	void *packet_icmp_head = rcv_packet + sizeof(struct mac_header) + sizeof(struct ip_header);
	struct icmp_echo* packet_icmp_echo;
	struct icmp_time_exceeded* packet_icmp_time_exceeded;
	//get type
	int *icmp_type = packet_icmp_head;
	*icmp_type = *icmp_type & 0xFF;
	//if echo
	if(*icmp_type == 0){
		packet_icmp_echo = packet_icmp_head;
		if(packet_icmp_echo->id != id)
			return -1;
		else
		{
			char *ip_str = get_ip(ntohl(packet_ip_head->ip_src));
			printf("%d) %s\n", *counter, ip_str);
			free(ip_str);
			return 0;
		}
	}
	//if time_exceeded
	else if(*icmp_type == 11){
		packet_icmp_time_exceeded = packet_icmp_head;
		if(packet_icmp_time_exceeded->icmp_echo.id != id)
			return -1;
		else
		{
			char *ip_str = get_ip(ntohl(packet_ip_head->ip_src));
			printf("%d) %s\n", *counter, ip_str);
			free(ip_str);
			return 1;
		}
	}
}

int main()
{
	char *buf = malloc(MAX_STR_LEN);
	struct sockaddr_ll sockaddr;
	char mac_str_dst[ETH_ALEN] = {0x00, 0x0b, 0x2b, 0x70, 0xfe, 0xeb};
	char mac_str_src[ETH_ALEN] = {0x08, 0x00, 0x27, 0x00, 0x00, 0x00};
	//raw socket create
	int sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sfd == -1) {
		perror("Can't create raw socket");
		return -1;
	}
	sockaddr.sll_family = AF_PACKET;
	sockaddr.sll_protocol = htons(ETH_P_IP);
	sockaddr.sll_ifindex = if_nametoindex("enp0s3");
	sockaddr.sll_halen = ETH_ALEN;
	memcpy(sockaddr.sll_addr, mac_str_src, ETH_ALEN);
	socklen_t addrlen = sizeof(sockaddr);
	//eth header
	int packet_len = 0;
	struct mac_header machead;
	packet_len += sizeof(struct mac_header);
	memcpy(machead.mac_dst, mac_str_dst, ETH_ALEN);
	memcpy(machead.mac_src, mac_str_src, ETH_ALEN);
	machead.type = htons(ETH_P_IP);
	//ip header
	packet_len += sizeof(struct ip_header);
	struct ip_header iphead;
	iphead.ihl = 5;
	iphead.version = 4;
	iphead.dscp = 0;
	iphead.ecn = 0;
	iphead.total_len = htons(packet_len - sizeof(struct mac_header) + sizeof(struct icmp_echo));
	iphead.identification = htons(1234);
	iphead.flags = 0;
	iphead.offset = 0;
	iphead.ttl = 0;
	iphead.protocol = IPPROTO_ICMP;
	iphead.csum = 0;
	iphead.ip_src = inet_addr(IP_SRC);
	iphead.ip_dst = inet_addr(IP_DST);
	iphead.csum = csum_calc((unsigned short int*)&iphead, sizeof(struct ip_header));
	//icmp header
	struct icmp_echo icmpecho;
	packet_len += sizeof(struct icmp_echo);
	icmpecho.type = 8;
	icmpecho.code = 0;
	icmpecho.csum = 0;
	int id = (int)getpid();
	icmpecho.id = id;
	icmpecho.seq = 1;
	icmpecho.payload = 0;
	icmpecho.csum = csum_calc((unsigned short int*)&icmpecho, sizeof(struct icmp_echo));
	//concat packet
	void *snd_packet = malloc(packet_len);
	void *rcv_packet = malloc(UDP_RESERVE);
	//send
	int counter = 0;
	int finished = 0;
	do {
		counter++;
		iphead.ttl++;
		iphead.csum = 0;
		iphead.csum = csum_calc((unsigned short int*)&iphead, sizeof(struct ip_header));
		memcpy(snd_packet, &machead, sizeof(machead));
		memcpy(snd_packet + sizeof(machead), &iphead, sizeof(iphead));
		memcpy(snd_packet + sizeof(machead) + sizeof(iphead), &icmpecho, sizeof(icmpecho));
		sendto(sfd, snd_packet, packet_len, 0, (struct sockaddr*)&sockaddr, addrlen);
		do {
			recvfrom(sfd, rcv_packet, UDP_RESERVE, 0, NULL, NULL);
			finished = print_packet(rcv_packet, id, &counter);
		}
		while(finished < 0);
	}
	while(finished != 0);
	printf("TRACE SUCCESS\n");
	//close
	close(sfd);
	free(buf);
	free(snd_packet);
	free(rcv_packet);
	return 0;
}