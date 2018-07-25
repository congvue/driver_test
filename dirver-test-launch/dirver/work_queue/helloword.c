#if 1
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
//#include <asm/jzsoc.h>

#include <linux/pm.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock_types.h>
#include <linux/semaphore.h>
#include <linux/string.h>

#include <linux/workqueue.h>


struct my_struct_t{
	char *name;
	struct work_struct my_work;
};


void my_func(struct work_struct *work)
{
	struct my_struct_t *my_name = container_of(work, struct my_struct_t, my_work);

	printk(KERN_INFO "HELLO WORLD MY NAME IS %s\n",my_name->name);

}


struct my_struct_t my_name;



int test_init(void)
{


	INIT_WORK(&my_name.my_work, my_func);

	my_name.name = "wyf";

	msleep(10 * 1000);

		schedule_work(&my_name.my_work);

	return 0;


}


void test_exit(void)
{
	
printk("aaaaaaaaaa\n");

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");


#endif




#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
//#include <asm/jzsoc.h>

#include <linux/pm.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock_types.h>
#include <linux/semaphore.h>
#include <linux/string.h>

#include <linux/workqueue.h>

struct my_work_queue{
	int test;
	struct work_struct save;
};

struct my_work_queue test_work;
struct workqueue_struct *test_workqueue;

void do_save(struct work_struct *p_work)
{
	struct my_work_queue *p_test_work = container_of(p_work, my_work_queue, save);
	printk("%d\n",p_test_work->test);

}

void test_init()
{
	test_workqueue = create_queue("test_workqueue");

	if(!test_workqueue)
		printk("create test_workqueue failed\n");

	INIT_WORK(&test_work.save, do_save);

	queue_work(test_workqueue, &test_work.save));

}


void test_exit(void)
{
	if(test_workqueue)
		destroy_workqueue(test_workqueue);

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");

