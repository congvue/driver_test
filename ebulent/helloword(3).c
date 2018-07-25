#if 0

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

unsigned int major = 253;
struct file_operations test_fops;

static int  __init test_init(void)
{
	register_chrdev(major, "test_driver", &test_fops);

	printk("hello kernel\n");
	
	return 0;
}

static void __exit test_exit(void)
{
	unregister_chrdev(major, "test_driver");

	printk("bye kernel\n");
	

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
MODULE_VERSION("V1.0");

#endif

#if 01

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
	printk("open success\n");
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


#if 0
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/sizes.h>

struct _wdt_t {
	unsigned long phy, virt;
	unsigned long *wdt_wcr,*wdt_wsr,*wdt_wrst, *wdt_dmcr;
	unsigned long reg;
	void (*init_restrt)(struct _wdt_t *);
};

struct _wdt_t my_wdt;

static void fsl_wdt_init_reset(struct _wdt_t *wdt)
{
	printk("restet....\n");

}


void init_wdt_device(struct _wdt_t *wdt)
{
	int ret = 0;
	int a, b, c,d;

	wdt->phy = 0x20bc000;
	wdt->virt = (unsigned long)ioremap(wdt->phy, 0x08);
	printk("virt = 0x%lx\n",wdt->virt);
	

	wdt->wdt_wcr =(unsigned long *) (wdt->virt + 0x0);
	printk("Line:%d\n",__LINE__);
	wdt->wdt_wsr = (unsigned long *) (wdt->virt + 0x2);
	printk("Line:%d\n",__LINE__);
	wdt->wdt_wrst = (unsigned long *)( wdt->virt + 0x4);
	wdt->wdt_dmcr = (unsigned long *) (wdt->virt + 0x6);
	printk("1111111111111\n");
	printk("Line:%d\n",__LINE__);

	a = __raw_readl(wdt->wdt_wcr);
	printk("Line:%d\n",__LINE__);
	b = __raw_readl(wdt->wdt_wsr);
	c = __raw_readl(wdt->wdt_wrst);
	d = __raw_readl(wdt->wdt_dmcr);

	wdt->init_restrt = fsl_wdt_init_reset;
	

	printk("wcr = 0x%x\n wsr = 0x%x\n wrsr = 0xx\n dmcr = 0x%x\n",a,b,c, d);

}

void destroy_wdt_device(struct _wdt_t *wdt)
{
	iounmap((void *)wdt->virt);

}

static int __init wdt_init(void)
{
	printk("AAAA\n");
	init_wdt_device(&my_wdt);
	my_wdt.init_restrt(&my_wdt);

	printk("hello wdt\n");
	return 0;

}


static void  __exit wdt_exit(void)
{
	destroy_wdt_device(&my_wdt);
	printk("bye wdt\n");

}


module_init(wdt_init);
module_exit(wdt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
MODULE_VERSION("V1.0");

#endif

#if 0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/io.h>

#include <linux/device.h>
#include <linux/miscdevice.h>

void led_on()
{
	printk("led is on\n");
}

void led_off()
{
	printk("led is off\n");

}

static int led_ioctl(struct inode *node, struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd) {
		case 0:
			led_on();
			break;
		case 1:
			led_off();
			break;
		default:
			printk("ioctl failed\n");
			return -1;
	}

	return 0;

}

static struct file_operations led_fops = {
	.unlocked_ioctl = led_ioctl,
};

static struct miscdevice misc_led = {
	.name = "test_led",
	.fops = &led_fops,
};

int test_init(void)
{
	misc_register(&misc_led);

	return 0;

}


void test_exit(void)
{
	misc_deregister(&misc_led);

	printk("bye kernel\n");

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");


#endif


#if 01
//这是测试frambuffer的mmap()函数的驱动程序

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/ctype.h>
#include <linux/pagemap.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include "demo.h"


struct simple_dev *simple_device;
static unsigned char simple_inc = 0;
static char *buffer = NULL;
static char *buffer_area = NULL;

int simple_open(struct inode *inode, struct file *filp)
{
	struct simple_dev *dev;
	simple_inc++;

	dev = container_of(inode->i_cdev, struct simple_dev, cdev);

	filp->private_data = dev;

	return 0;

}

int simple_release(struct inode *inode, struct file *filp)
{
	simple_inc--;

	return 0;
	
}

static int simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void *)((unsigned long)buffer_area)) >> PAGE_SHIFT,
		vma->vm_end - vma->vm_start, PAGE_SHARED);
/*
virt_to_phys((void *)((unsigned long)buffer_area)) 是将一段虚拟内存转换为物理内存
然后通过>> PAGE_SHIFT获得页帧号

*/

	if(ret != 0) {
		return -EAGAIN;
	}

	return 0;
	
}

struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.open = simple_open, 
	.mmap = simple_mmap,
	.release = simple_release,
};

void simple_cleanup_module(void)
{
	dev_t devno = MKDEV(simple_MAJOR, simple_MINOR);
	unsigned long virt_addr;

	if(simple_device){
		cdev_del(&simple_device->cdev);
		kfree(simple_device);
	}

	for(virt_addr = (unsigned long)buffer_area; virt_addr < (unsigned long)buffer_area + 4096; virt_addr += PAGE_SIZE) {
		SetPageReserved(virt_to_page(virt_addr));
	}

	if(buffer) kfree(buffer);

	unregister_chrdev_region(devno, 1);
	

}

int simple_init_module(void)
{
	int result;
	dev_t dev = 0;

	int i;
	unsigned long virt_addr;

	dev = MKDEV(simple_MAJOR, simple_MINOR);
	result = register_chrdev_region(dev, 1, "mmap");
	if(result < 0) {
		printk("register chrdev fail\n");
		goto out_free;
	}

	printk("major = %d minor = %d\n",MAJOR(dev), MINOR(dev));

	simple_device = kmalloc(sizeof(struct simple_dev), GFP_KERNEL);
	if(!simple_device) {
		printk("kmalloc mem to simple_device fail\n");
		goto out_free;
	}

	memset(simple_device, 0, sizeof(struct simple_dev));

	sema_init(&simple_device->sem, 1);

	cdev_init(&simple_device->cdev, &simple_fops);

	simple_device->cdev.owner =  THIS_MODULE;
	simple_device->cdev.ops = &simple_fops;
	result = cdev_add(&simple_device->cdev, dev, 1);
	if(result) {
		printk("cdev add fail\n");
		goto out_free;
	}

	buffer = kmalloc(4096, GFP_KERNEL);
	printk("mmap buffer = %p\n",buffer);

	buffer_area = (int *)(((unsigned long)buffer + PAGE_SIZE - 1) & PAGE_MASK);

	for(virt_addr = (unsigned long)buffer_area; virt_addr < (unsigned long)buffer_area + 4096; virt_addr += PAGE_SIZE) {
		SetPageReserved(virt_to_page(virt_addr));//将页面设置为保留，防止被映射到用户空间的页面被swap out 出去
	}

	memset(buffer, 'C', 100);

	return 0;

out_free:
	simple_cleanup_module();
	return result;
}

module_init(simple_init_module);
module_exit(simple_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");


#endif

