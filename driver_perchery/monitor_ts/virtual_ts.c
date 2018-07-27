#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock_types.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define SCREEN_MAX_X			800
#define SCREEN_MAX_Y			480

struct virtual_ts_data 
{
	struct cdev cdev;
	char kbuf[8];
	dev_t devno;
	unsigned short x;
	unsigned short y;
	unsigned short pressure;
	bool		   pendown;
	bool 		   LastState;

};


static struct input_dev *uk_dev;
struct class *my_class;
struct virtual_ts_data ts_dev;


static int input_init(void);
static int  report_value(unsigned char *data);
static int ts_open(struct inode *node, struct file *filp);
static int __init ts_init(void);
static void __exit ts_exit(void);


static int input_init(void)
{
	int ret ;
	/* a. 分配一个input_dev */

	uk_dev = input_allocate_device();
	/* b. 设置 */
	/* b.1 能产生哪类事件 */

	set_bit(EV_ABS, uk_dev->evbit);
	set_bit(EV_KEY, uk_dev->evbit);

	/* b.2 能产生哪些事件 */	
	set_bit(ABS_X, uk_dev->absbit);
	set_bit(ABS_Y, uk_dev->absbit);
	set_bit(ABS_PRESSURE, uk_dev->absbit);
	set_bit(BTN_TOUCH, uk_dev->keybit); 

	input_set_abs_params( uk_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params( uk_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params( uk_dev, ABS_PRESSURE, 0, 255, 0 , 0);

	uk_dev->name = "virtual_ts";
	uk_dev->phys = "touchscreen";
	uk_dev->id.bustype = BUS_HOST;
	uk_dev->id.vendor = 0x0001;
	uk_dev->id.product = 0x0001;
	uk_dev->id.version = 0x0001;

	/* c. 注册 */
	ret = input_register_device(uk_dev);

	if(ret != 0)
		printk("Register input device fail!\n");

	return ret;
}

static int  report_value(unsigned char *data)
{
	sscanf(data, "%hd,%hd,%d",ts_dev.x,ts_dev.y,ts_dev.pendown);

	if(ts_dev.pendown){
		if (ts_dev.pendown && !ts_dev.LastState) {
			input_report_key(input, BTN_TOUCH, 1);
			input_report_abs(uk_dev, ABS_X, ts_dev.x);
			input_report_abs(uk_dev, ABS_Y, ts_dev.y);
			input_report_abs(uk_dev, ABS_PRESSURE, 250);

			input_sync(uk_dev);
			ts_dev.LastState = 1;
		}
	}else if(ts_dev.pendown && ts_dev.LastState){
			input_report_abs(uk_dev, ABS_X, ts_dev.x);
			input_report_abs(uk_dev, ABS_Y, ts_dev.y);
			input_report_abs(uk_dev, ABS_PRESSURE, 250);

			input_sync(uk_dev);
	}
	
	if (!ts_dev->pendown) {
		input_report_key(uk_dev, BTN_TOUCH, 0);
		input_report_abs(uk_dev, ABS_PRESSURE, 0);
		input_sync(uk_dev);
		ts_dev.LastState = 0;
	}
	
}


int ts_open(struct inode *node, struct file *filp)
{	
	struct virtual_ts_data *dev;
	
	dev = container_of(node->i_cdev, struct virtual_ts_data, cdev);

	filp->private_data = dev;
	
	return 0;

}

int ts_release(struct inode *inode, struct file *filp)
{
	return 0;

}


ssize_t ts_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret ;
	struct virtual_ts_data *dev_data = filp->private_data;

	char *ptr1 = dev_data->kbuf;
	char *ptr2 = NULL;

	if(count > 9){
		printk("Error data size ! data size <  10\n");
		return -1;
	}

	if(copy_from_user(dev_data->kbuf, buf, count)){
		
		ret = -EFAULT;
		
	} else {	
		ptr2 = strstr(ptr1, ",");
		if(ptr2 == NULL){
			printk("Wrong data format! Format [unsigned short],[unsigned short]\n");
			return -1;
		}
		
		report_value(dev_data->kbuf);
	
	}

	return ret;

}


static const struct file_operations ts_fops = {
	.owner 		= THIS_MODULE,
	.open		= ts_open,
	.write 		= ts_write,
	.release 	= ts_release,
};


static int __init ts_init(void)
{	
	int ret;

	input_init();

	memset(&ts_dev, 0, sizeof(struct virtual_ts_data));

	ts_dev.pendown = false;
	ts_dev.LastState = false;

	ret = alloc_chrdev_region(&ts_dev.devno, 0, 1, "ts");

	printk("major[%d], minor[%d]\n",MAJOR(ts_dev.devno),MINOR(ts_dev.devno));
	
	cdev_init(&ts_dev.cdev, &ts_fops);
	
	ts_dev.cdev.owner 	= THIS_MODULE;
	
	ret = cdev_add(&ts_dev.cdev, ts_dev.devno, 1);

	my_class = class_create(THIS_MODULE, "my_class");
	
	device_create(my_class, NULL, ts_dev.devno, NULL, "vir_ts");

	printk("Hello Kernel\n");

	return 0;		

}


static void __exit ts_exit(void)
{
	cdev_del(&ts_dev.cdev);

	device_destroy(my_class, ts_dev.devno);

	class_destroy(my_class);

	unregister_chrdev_region(ts_dev.devno, 1);

	input_unregister_device(uk_dev);

	printk("Bye Kernel\n");

}


module_init(ts_init);
module_exit(ts_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
MODULE_VERSION("V1.0");
