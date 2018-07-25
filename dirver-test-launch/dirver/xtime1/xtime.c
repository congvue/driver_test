#include <linux/module.h>
#include <linux/kernel.h>


#include <linux/mm.h>  
#include <linux/miscdevice.h>  
#include <linux/slab.h>  
#include <linux/vmalloc.h>  
#include <linux/mman.h>  
#include <linux/random.h>  
#include <linux/init.h>  
#include <linux/raw.h>  
#include <linux/tty.h>  
#include <linux/capability.h>  
#include <linux/ptrace.h>  
#include <linux/device.h>  
#include <linux/highmem.h>  
#include <linux/crash_dump.h>  
#include <linux/backing-dev.h>  
#include <linux/bootmem.h>  
#include <linux/splice.h>  
#include <linux/pfn.h>  
#include <linux/export.h>  
  
#include <asm/uaccess.h>  
#include <asm/io.h> 


#define GET_XTIME		0

#if 0
unsigned long get_second(void)
{
	return timekeeper.xtime.tv_sec;

}


EXPORT_SYMBOL(get_second);
#endif



static int dev_open_xtime(struct inode *inode , struct file *filp)
{

	return 0;
}


static int dev_read_xtime(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return 0;

}

static ssize_t dev_write_xtime(struct file *file, char  __user *buf, size_t count, loff_t *ppos)
{
	return 0;
	
}

static long dev_ioctl_xtime(struct file *file , unsigned int cmd, unsigned long arg)
{
	int __user *argp = (int __user *)arg;
	unsigned long now = 0;

	switch(cmd){
		case GET_XTIME:
			now = get_seconds();
			if(copy_to_user(argp, &now,4))
				return -EFAULT;
			break;
		default:
			return -EFAULT;
	}
	return 0;
	
}	

static const struct file_operations dev_fops_xtime = {
	.read = dev_read_xtime,
	.write = dev_write_xtime,
	.open = dev_open_xtime,
	.unlocked_ioctl = dev_ioctl_xtime,
};

static struct cdev xtime_dev;
static int major;

static int __init char_dev_init(void)
{
	int rc;
	int err;
	dev_t devid;

	rc = alloc_chrdev_region(&devid, 0, 1, "xtime_dev");
	if(0 != rc){
		printk("alloc char dev failed\n");
		return -1;
	}

	major = MAJOR(devid);
	int minor = MINOR(devid);
	printk("major = %d minor = %d\n",major, minor);

	cdev_init(&xtime_dev, &dev_fops_xtime);
	cdev_add(&xtime_dev, devid, 1);

	return 0;
	

}


static void __exit char_dev_exit(void)
{
	cdev_del(&xtime_dev);
	unregister_chrdev_region(MKDEV(major, 0),1);

}




module_init(char_dev_init);
module_exit(char_dev_exit);
MODULE_LICENSE("GPL");




