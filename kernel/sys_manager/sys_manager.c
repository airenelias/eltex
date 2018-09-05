#include <linux/init.h> //init / exit
#include <linux/module.h> //module
#include <linux/sysfs.h> //proc fs
#include <linux/uaccess.h> //copy to userspace
#include <linux/kernel.h> //kstrtoint

#define SYS_ENTRY_FILENAME "bufint"
#define MAX_DIGITS 10

MODULE_LICENSE("GPL");

static int numeric_buffer;
static int fd;
static struct kobject *my_kobj;
static struct attribute my_attr;
static struct kobj_attribute my_kobj_attr;

static ssize_t sys_manager_read(struct kobject *, struct kobj_attribute *, char *);
static ssize_t sys_manager_write(struct kobject *, struct kobj_attribute *, const char *, size_t);

static int __init sys_manager_init(void) {
	printk(KERN_INFO "sys_manager: Initializing\n");

	my_attr.name = SYS_ENTRY_FILENAME;
	my_attr.mode = 0666;
	my_kobj_attr.attr = my_attr;
	my_kobj_attr.show = sys_manager_read;
	my_kobj_attr.store = sys_manager_write;

	my_kobj = kobject_create_and_add(SYS_ENTRY_FILENAME, kernel_kobj);
	if(my_kobj == NULL) {
		printk(KERN_ERR "sys_manager: Cant create kobject\n");
		return -ENOMEM;
	}
	fd = sysfs_create_file(my_kobj, &my_kobj_attr.attr);
	if(fd != 0) {
		kobject_put(my_kobj);
		return -ENOMEM;
	}
	printk(KERN_INFO "sys_manager: File created\n");
	return 0;
}

static void __exit sys_manager_exit(void) {
	printk(KERN_INFO "sys_manager: Exiting\n");
	sysfs_remove_file(my_kobj, &my_kobj_attr.attr);
	kobject_put(my_kobj);
	printk(KERN_INFO "sys_manager: Exit succesfull\n");
}

static ssize_t sys_manager_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
	//static char num_buf[MAX_DIGITS];
	sprintf(buf, "%d", numeric_buffer);
	//if(copy_to_user(buf, num_buf, MAX_DIGITS) != 0) {
	//	printk(KERN_ERR "sys_manager: Data read failed\n");
	//	return -EFAULT;
	//}
	printk(KERN_INFO "sys_manager: Data was read %s\n", buf);
	return MAX_DIGITS;
}

static ssize_t sys_manager_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
	//static char num_buf[MAX_DIGITS];
	//if(count == 0 || count > MAX_DIGITS)
	//	count = MAX_DIGITS;
	//if(copy_from_user(num_buf, buf, count) != 0) {
	//	printk(KERN_ERR "sys_manager: Data write failed\n");
	//	return -EFAULT;
	//}
	if(kstrtoint(buf, 10, &numeric_buffer) != 0) {
		printk(KERN_ERR "sys_manager: Data convert failed\n");
		return -EFAULT;
	}
	printk(KERN_INFO "sys_manager: Data was written: %d\n", numeric_buffer);
	return MAX_DIGITS;
}

module_init(sys_manager_init);
module_exit(sys_manager_exit);
