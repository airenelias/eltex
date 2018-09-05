#include <linux/init.h> //init / exit
#include <linux/module.h> //module
#include <linux/proc_fs.h> //proc fs
#include <linux/uaccess.h> //copy to userspace
#include <linux/kernel.h> //kstrtoint

#define PROC_ENTRY_FILENAME "bufint"
#define MAX_DIGITS 10

MODULE_LICENSE("GPL");

static int numeric_buffer;
static struct proc_dir_entry *proc_file;
static struct file_operations fops;

static int proc_manager_open(struct inode *, struct file *);
static int proc_manager_release(struct inode *, struct file *);
static ssize_t proc_manager_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t proc_manager_write(struct file *, const char __user *, size_t, loff_t *);

static int __init proc_manager_init(void) {
	printk(KERN_INFO "proc_manager: Initializing\n");

	fops.open = proc_manager_open;
	fops.release = proc_manager_release;
	fops.read = proc_manager_read;
	fops.write = proc_manager_write;

	proc_file = proc_create(PROC_ENTRY_FILENAME, 0644, NULL, &fops);
	if(proc_file == NULL) {
		printk(KERN_ERR "proc_manager: Cant create proc\n");
		remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
		return -ENOMEM;
	}
	printk(KERN_INFO "proc_manager: Proc created\n");
	return 0;
}

static void __exit proc_manager_exit(void) {
	printk(KERN_INFO "proc_manager: Exiting\n");
	remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
	printk(KERN_INFO "proc_manager: Exit succesfull\n");
}

//int (*open) (struct inode *, struct file *);
static int proc_manager_open(struct inode *inode, struct file *file) {
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "proc_manager: Open\n");
	return 0;
}
//int (*release) (struct inode *, struct file *);
static int proc_manager_release(struct inode *inode, struct file *file) {
	module_put(THIS_MODULE);
	printk(KERN_INFO "proc_manager: Release\n");
	return 0;
}

//ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
static ssize_t proc_manager_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos) {
	static char num_buf[MAX_DIGITS];
	if(nbytes == 0 || nbytes > MAX_DIGITS)
		nbytes = MAX_DIGITS;
	sprintf(num_buf, "%d", numeric_buffer);
	if(copy_to_user(buf, num_buf, nbytes) != 0) {
		printk(KERN_ERR "proc_manager: Data read failed\n");
		return -EFAULT;
	}
	printk(KERN_INFO "proc_manager: Data was read\n");
	return MAX_DIGITS;
}

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
static ssize_t proc_manager_write(struct file *file, const char __user *buf, size_t nbytes, loff_t *ppos) {
	static char num_buf[MAX_DIGITS];
	if(nbytes == 0 || nbytes > MAX_DIGITS)
		nbytes = MAX_DIGITS;
	if(copy_from_user(num_buf, buf, nbytes) != 0) {
		return -EFAULT;
	}
	if(kstrtoint(num_buf, 10, &numeric_buffer) != 0) {
		return -EFAULT;
	}
	printk(KERN_INFO "proc_manager: Data was written: %d\n", numeric_buffer);
	return MAX_DIGITS;
}

module_init(proc_manager_init);
module_exit(proc_manager_exit);
