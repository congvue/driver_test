#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/idr.h>
#include <linux/mutex.h>
#include <linux/of_device.h>
#include <linux/completion.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
#include <linux/rwsem.h>
#include <linux/pm_runtime.h>
#include <asm/uaccess.h>





extern struct bus_type i2c_bus_type;



static int __init test_init(void)
{
	printk("i2c_bus_type ->name = %s\n",i2c_bus_type.name);

	struct subsys_private *bus_priv = i2c_bus_type.p;


	return 0;
}

static void __exit test_exit(void)
{

}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xoao bai");
MODULE_VERSION("v0.1");




