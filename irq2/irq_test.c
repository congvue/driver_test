#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/irq.h>



int key_col1_gpio;


static irqreturn_t key_col1_irq_handler(int irq, void *dev_id)
{
    printk("key_col1 IRQ Handler\n");

    return IRQ_HANDLED;

}



static int key_col1_init()
{

	
}

static void key_col1_exit()
{


}
module_init(key_col1_init);
module_exit(key_col1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("percherry");



#if 0


/*定义一个处理函数*/ 
static void gt9xx_workqueue_func(struct work_struct *work)
{
	printk("########### gt9xx_workqueue_func######\n");
	msleep(1000);

	int irq = GT9XX_IRQ;
	gt9xx_irq_enable( irq);

}

ssize_t gt9xx_read(struct file *filp , char __user *buf,
					size_t count, loff_t *ppos)
{
	printk("########## GT9XX read ...........\n");

	return 0;

}

static struct file_operations gt9xx_ops = {
	.owner = THIS_MODULE,
	.read  = gt9xx_read,
};

static struct miscdevice gt9xx_miscdev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name 	= DEV_NAME,
	.fops	= &gt9xx_ops,
	.nodename = DEV_NAME,
};


static int gt9xx_init(void)
{

	memset(&gt_data, 0, sizeof(gt_data));

	gt_data.gt9xx_irq_num = GT9XX_IRQ;
	gt_data
	gpio_request(GT9XX_IRQ_PIN,"gp9xx_irq_pin");
	gpio_direction_input(GT9XX_IRQ_PIN);

	printk("kskgja\n");
	int ret = request_irq(GT9XX_IRQ, gt9xx_irq,
			  IRQ_TYPE_EDGE_RISING, "gt9xx_irq",NULL);
	if(ret){
		printk("request GT9XX irq fail\n");
		return ret;
	}

	spin_lock_init(&irq_lock_test);


//	通过INIT_WORK()可以初始化这个工作队列并将工作队列与处理函数绑定 

	INIT_WORK(&my_work, gt9xx_workqueue_func);

	my_wq = create_singlethread_workqueue("my_wq");
	if(!my_wq){
		printk("Create work queue fail\n");
		return -ENOMEM;
	}

	ret = misc_register(&gt9xx_miscdev);
	if(ret){
		printk("gt9xx_miscdev register fail\n");
		return ret;
	}

	return 0;

}

static void gt9xx_exit(void)
{
	misc_deregister(&gt9xx_miscdev);
}
#endif
