#if 1 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#include <asm/uaccess.h>
#include <linux/errno.h>

struct ioctl_data{
	unsigned int size;
	char buf[100];
};


#define TEST_MAGIC 		'X'
#define TEST_MAX_NR		3

#define TEST_CLEAR		_IO(TEST_MAGIC ,1)
#define TEST_OFFSET		_IO(TEST_MAGIC, 2)
#define TEST_KBUF		_IOW(TEST_MAGIC, 3, struct ioctl_data);


#define P_DEBUG(fmt, args...) 	printk("<1>" "<kernel> [%s]" fmt, __FUNCTION__, ##args)

#define DEV_SIZE	100

volatile static int time_write = 0;

struct _test_t{
	char kbuf[DEV_SIZE];
	unsigned int major;
	unsigned int minor;
	unsigned int cur_size;
	dev_t devno;
	struct cdev test_cdev;
};

int test_open(struct inode *node, struct file *filp)
{
	struct _test_t *dev;
	dev = container_of(node->i_cdev, struct _test_t, test_cdev);
	
	filp->private_data = dev;
	printk("openSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS success\n");
	return 0;

}

int test_close(struct inode* node, struct file *filp)
{
	return 0;

}

ssize_t test_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	struct _test_t *dev = filp->private_data;

	if(*offset >= DEV_SIZE) {
		printk("offset > DEV_SIZE\n");
		return count ? -ENXIO : 0;
	}

	if(*offset + count > DEV_SIZE) {
		count = DEV_SIZE - *offset;
	}

	if(copy_to_user(buf, dev->kbuf + *offset, count) ){
		ret = -EFAULT;
	} else {
		ret = count;
		dev->cur_size -= count;
		*offset += count;
		P_DEBUG("read %d bytes cursize = %d\n",count, dev->cur_size);
	}

	return ret;
}

ssize_t test_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret;
	struct _test_t *dev = filp->private_data;
	time_write++;

	printk("*offset = %d  times = %d\n",*offset, time_write);

	if(*offset > DEV_SIZE) {
		printk("*offset > DEV_SIZE\n");
		return count ? -ENXIO : 0;
	}

	if(*offset + count > DEV_SIZE) {
		count = DEV_SIZE - *offset;
	}
	
	if(copy_from_user(dev->kbuf + *offset, buf, count)) {
		ret = -EFAULT;
	}else {
	ret = count; 
	dev->cur_size += count;
	*offset += count;
	P_DEBUG("Write %d bytes cur_size = %d\n",count, dev->cur_size);
	}

	return ret;

}


loff_t test_llseek(struct file *filp, loff_t offset, int whence)
{
	loff_t new_pos;
	loff_t old_pos = filp->f_pos;

	switch(whence) {
		case SEEK_SET:
			new_pos = offset;
			break;
		case SEEK_CUR:
			new_pos = old_pos + offset;
			break;
		case SEEK_END:
			new_pos = DEV_SIZE + offset;
			break;
		default:
			printk("unknow whence\n");
			return -EINVAL;
	}

	if(new_pos < 0 || new_pos > DEV_SIZE) {
		return -EINVAL;
	}

	filp->f_pos = new_pos;

	return new_pos;
}


struct file_operations test_fops = {
	.open = test_open,
	.release = test_close,
	.write = test_write,
	.read = test_read,
	.llseek = test_llseek,
};

struct _test_t my_dev;
struct class *my_class;

static int __init test_init(void)
{
	int result = 0;
	my_dev.cur_size = 0;
	my_dev.major = 0;
	my_dev.minor = 0;
	
	printk("\n 88888888888888888888888888\n");

	if(my_dev.major){
		my_dev.devno = MKDEV(my_dev.major, my_dev.minor);
		result = register_chrdev_region(my_dev.devno, 1, "test new driver");
	}else {
	result = alloc_chrdev_region(&my_dev.devno, my_dev.minor, 1, "test alloc dirver");

	my_dev.major = MAJOR(my_dev.devno);
	my_dev.minor = MINOR(my_dev.devno);
	}

	printk("major[%d], minor[%d]\n",my_dev.major, my_dev.minor);

	cdev_init(&my_dev.test_cdev, &test_fops);
	my_dev.test_cdev.owner = THIS_MODULE;
	result = cdev_add(&my_dev.test_cdev, my_dev.devno, 1);

	my_class = class_create(THIS_MODULE, "my_calss");
	device_create(my_class, NULL, my_dev.devno, NULL,"hello");

	printk("hello kernl\n");

	return 0;

}

static void __exit test_exit(void)
{
	cdev_del(&my_dev.test_cdev);

	device_destroy(my_class, my_dev.devno);

	class_destroy(my_class);

	unregister_chrdev_region(my_dev.devno, 1);
	
	printk("good bye kernel\n");

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
MODULE_VERSION("V1.0");



#endif








#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#include <asm/uaccess.h>
#include <linux/errno.h>

