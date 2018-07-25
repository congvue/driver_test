#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#include <asm/uaccess.h>
#include <linux/errno.h>


#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO11 	0x20e0040
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO11    0x20e0388



void __iomem *mux_ctl = NULL;
void __iomem *pad_ctl = NULL;


#define  MISC_NAME 		"beeper"

static int beeper_open(struct inode *inode , struct file *file)
{

	int data = 0;
#if 1
	mux_ctl  = ioremap(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO11, 4);
	if(mux_ctl ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}

	pad_ctl = ioremap(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO11, 4);
	if(pad_ctl ==NULL){
		printk("ioreamp fail\n");
		return -1;
	}

	//*mux_ctl = 2;//0010 ALT2 ¡ª Select signal PWM2_OUT.
	//*pad_ctl = 0x800; //	   1 ENABLED ¡ª Output is Open Drain.

	data = readl(pad_ctl);
	printk("data = %d\n",data);
	data = readl(mux_ctl);
	printk("data2 = %d\n",data);

#endif
	printk("55555555555beeper...\n");

}


static ssize_t beeper_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{


	printk("333333333333333333333\n");
	int ret;


	if(copy_to_user(buf, "beeper", count) ){
		ret = -EFAULT;
	} else {

		printk("read  bytes cursize = %d\n",count);
	}

	return ret;


}



static ssize_t beeper_write(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{

	printk("444444444444444\n");
	int ret;
	char buff[20];
	if(copy_from_user(buff, buf, count)) {
		ret = -EFAULT;
	}else {

	printk("Write %d bytes cur_size = %d\n",count, count);
	}

	return ret;


}






static const struct file_operations beeper_fops = {
	.owner = THIS_MODULE,
	.open = beeper_open,
	.read =beeper_read,
	.write = beeper_write,
};


static struct miscdevice beeper_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_NAME,
	.fops = &beeper_fops,
};

static int __init beeper_init(void)
{
	int ret ;
	ret = misc_register(&beeper_dev);

	if(ret){
		printk("misc_register error\n");
		return ret;
	}
	printk("lllllllllllllllll\n");

	return 0;

}


static void __exit beeper_exit(void)
{
	misc_deregister(&beeper_dev);
	printk("22222222222222\n");

}


module_init(beeper_init);
module_exit(beeper_exit);
MODULE_LICENSE("GPL");



