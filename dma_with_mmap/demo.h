#include <linux/ioctl.h>

#define simple_MAJOR	224
#define simple_MINOR	0
#define COMMAND1 		1
#define COMMAND2 		2

struct simple_dev {
	struct semaphore sem;
	struct cdev cdev;
};

ssize_t simple_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t simple_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
loff_t simple_llseek(struct file *filp, loff_t off, int whence);
int simple_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);