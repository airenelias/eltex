#include <linux/init.h> //init / exit
#include <linux/module.h> //module
#include <linux/kernel.h> //kstrtoint
#include <linux/skbuff.h>
#include <net/checksum.h>
#include <net/net_namespace.h>
#include <net/flow.h>
#include <net/ip.h>
#include <linux/netfilter.h> //netfilter
#include <linux/netfilter_ipv4.h> //priorities
#include <linux/if_ether.h> //ethernet
#include <linux/ip.h> //ip header
#include <linux/icmp.h> //icmp header

#define IF_NAME "enp0s3"
#define IP_SRC "192.168.1.231"
#define IP_DST "195.93.187.17"

MODULE_LICENSE("GPL");

static int ping_send(struct sk_buff *skb);
static unsigned int inet_addr(char *str);

static int __init ping_init(void) {
	struct sk_buff *skb = NULL;
	pr_info("ping: Initializing\n");
	ping_send(skb);
	pr_info("ping: Sent\n");
	return 0;
}

static void __exit ping_exit(void) {
	pr_info("ping: Exit succesfull\n");
}

static int ping_send(struct sk_buff *skb) {
	struct iphdr *ip_head = NULL;
	struct icmphdr *icmp_head = NULL;

	struct net_device *netdev = NULL;
	struct rtable *route_table = NULL;
	struct flowi4 fl4;

	unsigned int size = 0;
	size = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct icmphdr);

	skb = alloc_skb(size, GFP_ATOMIC);
	if(skb == NULL) {
		pr_err("ping: Cannot alloc skb\n");
		return -EFAULT;
	}
	skb_reserve(skb, size);
	skb_push(skb, size - sizeof(struct ethhdr));
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, 0);
	skb_set_transport_header(skb, sizeof(struct iphdr));

	netdev = dev_get_by_name(&init_net, IF_NAME);
	fl4.saddr = inet_addr(IP_SRC);
	fl4.daddr = inet_addr(IP_DST);
	fl4.flowi4_oif = netdev->ifindex;

	route_table = ip_route_output_key(&init_net, &fl4);
	skb_dst_set(skb, &route_table->dst);
	skb->dev = netdev;

	ip_head = ip_hdr(skb);
	ip_head->ihl = sizeof(struct iphdr)/4;
	ip_head->version = 4;
	ip_head->tos = 0;
	ip_head->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	ip_head->id = htons(1234);
	ip_head->frag_off = 0;
	ip_head->ttl = 255;
	ip_head->protocol = IPPROTO_ICMP;
	ip_head->check = 0;
	ip_head->saddr = fl4.saddr;
	ip_head->daddr = fl4.daddr;

	icmp_head = icmp_hdr(skb);
	icmp_head->type = 8;
	icmp_head->code = 0;
	icmp_head->un.echo.id = 0;
	icmp_head->un.echo.sequence = 1;
	icmp_head->checksum = ip_compute_csum(icmp_head, sizeof(struct icmphdr));

	ip_local_out(&init_net, NULL, skb);

	return 0;
}

static unsigned int inet_addr(char *str) {
	int a,b,c,d;
	char arr[4];
	sscanf(str, "%d.%d.%d.%d", &a,&b,&c,&d);
	arr[0] = a; arr[1] = b; arr[2] = c; arr[3] = d;
	return *(unsigned int*) arr;
}

module_init(ping_init);
module_exit(ping_exit);
