#include <linux/init.h> //init / exit
#include <linux/module.h> //module
#include <linux/device.h> //devices
#include <linux/fs.h> //file system
#include <linux/uaccess.h> //copy to userspace
//#include <linux/kernel.h>

#define DEVICE_NAME "hello_device"
#define CLASS_NAME "hello"

MODULE_LICENSE("GPL");

static int major_number;
static struct class* class;
static struct device* device;
static int times_opened = 0;
static char msg[256];
static struct file_operations fops;

static int device_manager_open(struct inode *, struct file *);
static int device_manager_release(struct inode *, struct file *);
static ssize_t device_manager_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_manager_write(struct file *, const char __user *, size_t, loff_t *);

static int __init device_manager_init(void) {
	printk(KERN_INFO "device_manager: Initializing\n");

	fops.open = device_manager_open;
	fops.release = device_manager_release;
	fops.read = device_manager_read;
	fops.write = device_manager_write;

	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if(major_number < 0) {
		printk(KERN_ERR "device_manager: Failed to allocate major number\n");
		return major_number;
	}
	else
		printk(KERN_INFO "device_manager: Allocated major number %d\n", major_number);

	class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(class)) {
		unregister_chrdev(major_number, DEVICE_NAME);
		printk(KERN_ERR "device_manager: Failed to create class\n");
		return major_number;
	}
	else
		printk(KERN_INFO "device_manager: Created device class %d\n", major_number);

	device = device_create(class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
	if(IS_ERR(device)) {
		class_unregister(class);
		class_destroy(class);
		unregister_chrdev(major_number, DEVICE_NAME);
		printk(KERN_ERR "device_manager: Failed to create device\n");
		return major_number;
	}
	else
		printk(KERN_INFO "device_manager: Created device and registered %d\n", major_number);
	return major_number;
}

static void __exit device_manager_exit(void) {
	printk(KERN_INFO "device_manager: Exiting\n");
	device_destroy(class, MKDEV(major_number, 0));
	class_unregister(class);
	class_destroy(class);
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "device_manager: Exit succesfull\n");
}

//int (*open) (struct inode *, struct file *);
static int device_manager_open(struct inode *inode, struct file *file) {
	times_opened++;
	printk(KERN_INFO "device_manager: Open. Device opened %d times\n", times_opened);
	return 0;
}
//int (*release) (struct inode *, struct file *);
static int device_manager_release(struct inode *inode, struct file *file) {
	times_opened--;
	printk(KERN_INFO "device_manager: Release. Device opened %d times\n", times_opened);
	return 0;
}

//ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
static ssize_t device_manager_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos) {
	if(copy_to_user(buf, msg, nbytes) == 0) {
		printk(KERN_INFO "device_manager: Data was read\n");
		return nbytes;
	}
	else {
		printk(KERN_ERR "device_manager: Data read failed\n");
		return -1;
	}
}

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
static ssize_t device_manager_write(struct file *file, const char __user *buf, size_t nbytes, loff_t *ppos) {
	snprintf(msg, nbytes, "%s", buf);
	printk(KERN_INFO "device_manager: Data was written\n");
	return nbytes;
}

module_init(device_manager_init);
module_exit(device_manager_exit);
