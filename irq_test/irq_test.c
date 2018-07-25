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






#define DEV_NAME	"gt9xx"

/*定义一个工作队列*/
struct work_struct my_work;
spinlock_t irq_lock_test;
static struct workqueue_struct *my_wq;
static int irq_is_disable=0;

struct gt9xx_data{
	
	struct  work_struct gt9xx_work;

	struct i2c_client *client;

	struct input_dev * gt9xx_input_dev;
	
	spinlock_t gt9xx_irq_lock;
	
	struct workqueue_struct *gt9xx_wq;

	int gt9xx_irq_num;

	int gt9xx_rst_pin;
};


#define GTP_RST_PORT	              	IMX_GPIO_NR(2, 23)

#define GT9XX_IRQ_PIN	IMX_GPIO_NR(1, 2)
#define GT9XX_IRQ		gpio_to_irq(GT9XX_IRQ_PIN)

void gt9xx_irq_enable( int irq);
void gt9xx_irq_disable( int irq);






void gt9xx_irq_enable(int irq)
{	
	unsigned long irqflags = 0;
	irq = GT9XX_IRQ;


	spin_lock_irqsave(&irq_lock_test,irqflags);
	
	if(irq_is_disable){
		enable_irq(irq);
		irq_is_disable = 0;
	}
	spin_unlock_irqrestore(&irq_lock_test, irqflags);
	
}

void gt9xx_irq_disable(int irq)
{
	unsigned long irqflag = 0;

	irq  = 	GT9XX_IRQ;


	spin_lock_irqsave(&irq_lock_test, irqflag);
	
	if(!irq_is_disable){
		disable_irq_nosync(irq);
		irq_is_disable = 1;
	}
	
	spin_unlock_irqrestore(&irq_lock_test, irqflag);
	

}

static irqreturn_t gt9xx_irq(int irq, void *dev_id)
{


	printk("############## GT9XX_ IRQ............\n");
	//disable_irq(irq);
	gt9xx_irq_disable(irq);

	
	//schedule_work(&work);/*调度工作队列执行*/ 
	queue_work(my_wq, &my_work);
	

	return IRQ_HANDLED;

}

struct gt9xx_data   * gt_data = NULL;

static int gt9xx_probe(struct i2c_client *client, 
										const struct i2c_device_id *id)
{
	int ret = 0;

	printk("####### gt9xx  probe...\n");
	gt_data = (struct gt9xx_data *)kzalloc(sizeof(*gt_data), GFP_KERNEL);
	if(gt_data ==  NULL){
		printk("malloc fail\n");
		return -1;
	}

	gt_data->client = client;

	gt_data->gt9xx_irq_num = GT9XX_IRQ;
	
	gt_data->gt9xx_rst_pin = GTP_RST_PORT;

	i2c_set_clientdata(client, gt_data);

	ret = request_irq(gt_data->gt9xx_irq_num, 
					  gt9xx_irq, 
					  irq_table[0],
					  "gt9xx_irq",
					  gt_data);
	
	

	

}


static int gt9xx_remove(struct i2c_client  *client)
{
	struct gt9xx_data * data = i2c_get_clientdata(client);

	printk("GT9XX driver remove , ...\n");

	i2c_set_clientdata(client, NULL);
	input_unregister_device(data->gt9xx_input_dev);

}

#define GT9XX_NAME	"Goodix-TS"
static const struct i2c_device_id gt9xx_id[] = {
		{GT9XX_NAME, 0},
};

static struct i2c_driver gt9xx_driver  = {
	.probe = gt9xx_probe,
	.remove = gt9xx_remove,
	.id_table = gt9xx_id,
	.driver = {
		.name = GT9XX_NAME,
		.owner = THIS_MODULE,
	},
};

static int gt9xx_init()
{
	i2c_add_driver(&gt9xx_driver);
	
}

static void gt9xx_exit()
{
	i2c_del_driver(&gt9xx_driver);

}
module_init(gt9xx_init);
module_exit(gt9xx_exit);

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
