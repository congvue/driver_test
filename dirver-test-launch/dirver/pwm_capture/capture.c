/*
内存资源。
void * __ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags) 
入口： phys_addr：要映射的起始的IO地址； 
size：要映射的空间的大小； 
flags：要映射的IO空间的和权限有关的标志； 
功能： 将一个IO地址空间映射到内核的虚拟地址空间上去，便于访问

*/



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>


#include <asm/uaccess.h>
#include <linux/errno.h>


#define CAPTURE_MAGIC	'X'

#define CAPTURE_CLEAR	_IO(CAPTURE_MAGIC, 1)

#define SD1_DATA0	0x20e0228
#define SD1_GPT_CR 	0X2098000
#define SD1_GPT_PR	0X2098004
#define SD1_GPT_SR	0X2098008
#define SD1_GPT_ICR 0X209801C

unsigned char *sd1_data_addr = NULL;
unsigned char *sd1_gpt_cr_addr = NULL;
unsigned char *sd1_gpt_pr_addr = NULL;
unsigned char *sd1_gpt_sr_addr = NULL;
unsigned char *sd1_gpt_icr_addr = NULL;


struct capture_data_t {
	char buff[100];
	unsigned int major;
	unsigned int minor;
	unsigned int cur_size;
	dev_t devno;
	struct cdev capture_dev;
};

int capture_open(struct inode *inode, struct file *filp)
{
	struct capture_data *dev;
	dev = container_of(inode->i_cdev, struct capture_data_t, capture_dev);

	filp->private_data = dev;

	sd1_data_addr = ioremap(SD1_DATA0, 4);
	if(sd1_data_addr ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}
	sd1_gpt_cr_addr = ioremap(SD1_GPT_CR, 4);
	if(sd1_gpt_cr_addr ==NULL){
	printk("ioreamp fail\n");
	return -1;
	}

	

	sd1_gpt_pr_addr = ioremap(SD1_GPT_PR, 4);
	if(sd1_gpt_pr_addr ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}
	sd1_gpt_pr_addr = ioremap(SD1_GPT_PR, 4);
	if(sd1_gpt_pr_addr ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}

	sd1_gpt_icr_addr = ioremap(SD1_GPT_ICR, 4);
	if(sd1_gpt_icr_addr ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}
	
//	printf("\nDisable GPT and config Capture 1\n");
	sd1_gpt_cr_addr  = 0x540	

//fen ping shezhi
	sd1_gpt_pr_addr = 11;

	int reg = 0x

	return 0;

}

int capture_close(struct inode *node, struct file *filp)
{
	return 0;
}

ssize_t capture_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{


	return 0;

}

struct file_operations capture_fops = {
	.owner = THIS_MODULE,
	.open = capture_open,
	.release = capture_close,
	.read = capture_read,
};

struct capture_data_t capture_data;
struct class *capture_class;

static int __init capture_init(void)
{
	int ret = 0;
	capture_data.major = 0;
	capture_data.minor = 0;

	ret = alloc_chrdev_region(&capture_data.devno, capture_data.minor, 1, "capture_driver");

	capture_data.major = MAJOR(capture_data.devno);
	capture_data.minor = MINOR(capture_data.devno);

	printk("major = %d, minor = %d\n", capture_data.major, capture_data.minor);

	cdev_init(&capture_data.capture_dev, &capture_fops);

	capture_data.capture_dev.owner = THIS_MODULE;

	ret = cdev_add(&capture_data.capture_dev, capture_data.devno, 1);

	capture_class = class_create(THIS_MODULE, "capture_calss");
	device_create(capture_class, NULL, capture_data.devno, NULL, "capture");

	return 0;
}

static void __exit capture_exit(void)
{	
	cdev_del(&capture_data.capture_dev);
	device_destroy(capture_class, capture_data.devno);
	class_destroy(capture_class);
	unregister_chrdev_region(capture_data.devno, 1);

}


module_init(capture_init);
module_exit(capture_exit);

MODULE_LICENSE("GPL");
