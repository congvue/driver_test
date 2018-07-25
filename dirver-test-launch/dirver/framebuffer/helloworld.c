#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>


static int __init test_init(void)
{
			

}

static void __exit test_exit(void)
{
	

}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");

MODULE_VERSION("v1.0");

