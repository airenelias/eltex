#include <linux/init.h> //init / exit
#include <linux/module.h> //module
#include <linux/proc_fs.h> //proc fs
#include <linux/fs.h> //copy to userspace
#include <linux/kernel.h> //kstrtoint
#include <linux/netfilter.h> //netfilter
#include <linux/netfilter_ipv4.h> //priorities
#include <linux/ip.h> //ip header
#include <linux/udp.h> //udp header
#include <linux/tcp.h> //tcp header

#define PROC_ENTRY_FILENAME "portblock"
#define MAX_DIGITS 5

MODULE_LICENSE("GPL");

static int port_number;
static struct proc_dir_entry *proc_file;
static struct file_operations fops;
static struct nf_hook_ops hops;

static int port_block_open(struct inode *, struct file *);
static int port_block_release(struct inode *, struct file *);
static ssize_t port_block_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t port_block_write(struct file *, const char __user *, size_t, loff_t *);
static unsigned int port_block_block(void *, struct sk_buff *, const struct nf_hook_state *);


static int __init port_block_init(void) {
	printk(KERN_INFO "port_block: Initializing\n");

	fops.open = port_block_open;
	fops.release = port_block_release;
	fops.read = port_block_read;
	fops.write = port_block_write;

	hops.hook = port_block_block;
	hops.pf = PF_INET;
	hops.hooknum = NF_INET_PRE_ROUTING;
	hops.priority = NF_IP_PRI_FIRST;

	proc_file = proc_create(PROC_ENTRY_FILENAME, 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk(KERN_ERR "port_block: Cant create proc\n");
		remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
		return -ENOMEM;
	}
	printk(KERN_INFO "port_block: Proc created\n");
	if(nf_register_net_hook(&init_net, &hops) != 0) {
		printk(KERN_ERR "port_block: Cant register net hook\n");
		remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
		return -ENOMEM;
	}
	printk(KERN_INFO "port_block: Net hook created\n");
	return 0;
}

static void __exit port_block_exit(void) {
	printk(KERN_INFO "port_block: Exiting\n");
	nf_unregister_net_hook(&init_net, &hops);
	remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
	printk(KERN_INFO "port_block: Exit succesfull\n");
}

//int (*open) (struct inode *, struct file *);
static int port_block_open(struct inode *inode, struct file *file) {
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "port_block: Open\n");
	return 0;
}
//int (*release) (struct inode *, struct file *);
static int port_block_release(struct inode *inode, struct file *file) {
	module_put(THIS_MODULE);
	printk(KERN_INFO "port_block: Release\n");
	return 0;
}

//ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
static ssize_t port_block_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos) {
	char num_buf[MAX_DIGITS];
	int err = 0;
	sprintf(num_buf, "%d", port_number);
	err = simple_read_from_buffer(buf, MAX_DIGITS, ppos, num_buf, MAX_DIGITS);
	if(err <= 0) {
		printk(KERN_ERR "port_block: Data read failed:%d\n", err);
		return -EFAULT;
	}
	printk(KERN_INFO "port_block: Data was read: %s\n", buf);
	return MAX_DIGITS;
}

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
static ssize_t port_block_write(struct file *file, const char __user *buf, size_t nbytes, loff_t *ppos) {
	char num_buf[MAX_DIGITS];
	int err = 0;
	err = simple_write_to_buffer(num_buf, MAX_DIGITS, ppos, buf, MAX_DIGITS);
	if(err <= 0) {
		printk(KERN_ERR "port_block: Data write failed:%d\n", err);
		return -EFAULT;
	}
	if(kstrtoint(num_buf, 10, &port_number) != 0) {
		printk(KERN_ERR "port_block: Data convert failed\n");
		return -EFAULT;
	}
	port_number = htons(port_number);
	printk(KERN_INFO "port_block: Data was written: %d\n", port_number);
	return MAX_DIGITS;
}

static unsigned int port_block_block(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
	struct iphdr *ip_head;
	struct udphdr *udp_head;
	struct tcphdr *tcp_head;

	ip_head = ip_hdr(skb);
	if(ip_head->protocol == IPPROTO_UDP) {
		udp_head = udp_hdr(skb);
		if(udp_head==NULL) 
		printk(KERN_INFO "port_block: PORTLLOLLOLL \n");
		printk(KERN_INFO "port_block: PORT %d\n", udp_head->dest);
		if(udp_head->source == port_number || udp_head->dest == port_number) {
			printk(KERN_INFO "port_block: UDP packet blocked\n");
			return NF_DROP;
		}
		printk(KERN_INFO "port_block: UDP packet passed\n");
		return NF_ACCEPT;
	}
	if(ip_head->protocol == IPPROTO_TCP) {
		tcp_head = tcp_hdr(skb);
		if(tcp_head->source == port_number || tcp_head->dest == port_number) {
			printk(KERN_INFO "port_block: TCP packet blocked\n");
			return NF_DROP;
		}
		printk(KERN_INFO "port_block: TCP packet passed\n");
		return NF_ACCEPT;
	}
	printk(KERN_INFO "port_block: Nor TCP nor UDP packet passed\n");
	return NF_ACCEPT;
}
module_init(port_block_init);
module_exit(port_block_exit);
