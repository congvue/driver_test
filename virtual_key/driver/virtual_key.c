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
#include <linux/sched.h>


struct virtual_ts_data
{
	struct cdev cdev;
	char kbuf[15];
	dev_t devno;
	unsigned short x;
	unsigned short y;
	unsigned short pressure;
	unsigned int key_value;
	unsigned int	pendown;
	bool 		   LastState;

};

static struct input_dev *uk_dev;
struct class *my_class;
struct virtual_ts_data ts_dev;

static int input_init(void);
static int  report_value(unsigned int value);
static int ts_open(struct inode *node, struct file *filp);
static int __init ts_init(void);
static void __exit ts_exit(void);


static int input_init(void)
{
	int ret ;
	int i=0;

	uk_dev = input_allocate_device();


//	set_bit(EV_KEY, uk_dev->evbit);
//	set_bit(EV_MSC, uk_dev->evbit);
//	set_bit(EV_SYN, uk_dev->evbit);
//	set_bit(EV_ABS, uk_dev->evbit);
//	set_bit(EV_REP, uk_dev->evbit);
	
	uk_dev->evbit[0] = BIT_MASK(EV_KEY);


	//set_bit(MSC_SCAN, uk_dev->mscbit);
	//set_bit(KEY_ENTER,uk_dev->keybit);
	//set_bit(KEY_LEFT,uk_dev->keybit);
	//set_bit(KEY_RIGHT,uk_dev->keybit);
	//set_bit(KEY_UP,uk_dev->keybit);
	//set_bit(KEY_DOWN,uk_dev->keybit);
	//set_bit(KEY_ESC,uk_dev->keybit);

	for(i=1;i<128;i++){
		set_bit(i,uk_dev->keybit);
	}

	clear_bit(0, uk_dev->keybit);

	uk_dev->name = "virtual_ts";
	uk_dev->phys = "virtual_ts";
	uk_dev->id.bustype = BUS_HOST;
	uk_dev->id.vendor = 0x0001;
	uk_dev->id.product = 0x0001;
	uk_dev->id.version = 0x0001;


	ret = input_register_device(uk_dev);

	if(ret != 0)
		printk("Register input device fail!\n");

	return ret;
}

static int  report_value(unsigned int value)
{


	//BTN_TOUCH


	printk("value = 0x%2x\n",value);

	input_report_key(uk_dev, value, 1);

#if 1
	//input_event(uk_dev, EV_MSC, MSC_SCAN,70052 );

	input_sync(uk_dev);


	//input_event(uk_dev, EV_MSC, MSC_SCAN,70052 );
//	msleep(1);

	input_report_key(uk_dev, value, 0);
  
	input_sync(uk_dev);
#endif
	return 0;
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
	
	sscanf(buf, "%d", &dev_data->key_value);

	report_value(dev_data->key_value);

	return count;

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
	printk("kakjgak\n");

	memset(&ts_dev, 0, sizeof(struct virtual_ts_data));

	ts_dev.pendown = true;
	ts_dev.LastState = false;

	ret = alloc_chrdev_region(&ts_dev.devno, 0, 1, "ts");

	printk("major[%d], minor[%d]\n",MAJOR(ts_dev.devno),MINOR(ts_dev.devno));

	cdev_init(&ts_dev.cdev, &ts_fops);

	ts_dev.cdev.owner 	= THIS_MODULE;

	ret = cdev_add(&ts_dev.cdev, ts_dev.devno, 1);

	my_class = class_create(THIS_MODULE, "my_class");

	device_create(my_class, NULL, ts_dev.devno, NULL, "virtual_key");

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
