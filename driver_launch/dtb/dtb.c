#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/irq.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <linux/of_gpio.h>



static struct miscdevice misc1 = {
	.minor 		= MISC_DYNAMIC_MINOR,
	.name		= "misc_led",
	//.fops 		= &leds_fops,
};



#if 0


struct device_node {
	const char *name;
	const char *type;
	phandle phandle;
	const char *full_name;

	struct	property *properties;
	struct	property *deadprops;	/* removed properties */
	struct	device_node *parent;
	struct	device_node *child;
	struct	device_node *sibling;
	struct	device_node *next;	/* next device of same type */
	struct	device_node *allnext;	/* next in list of all nodes */
	struct	proc_dir_entry *pde;	/* this node's proc directory */
	struct	kref kref;
	unsigned long _flags;
	void	*data;
#if defined(CONFIG_SPARC)
	const char *path_component_name;
	unsigned int unique_id;
	struct of_irq_controller *irq_trans;
#endif
};

#define MAX_PHANDLE_ARGS 8
struct of_phandle_args {
	struct device_node *np;
	int args_count;
	uint32_t args[MAX_PHANDLE_ARGS];
};
struct property {
	char	*name;
	int	length;
	void	*value;
	struct property *next;
	unsigned long _flags;
	unsigned int unique_id;
};

#endif

void print_hex(void *data, long size)
{
	int i, j;
	for(i=0;i < size; i++){
		printk("0x%2x ",*(char *)data);
		data++;
	}

	printk("\n");

}

extern struct device_node *of_allnodes;
void print_property_name(struct device_node *node)
{
	struct property **tmp = &node->properties;

	while(1){
		if(*tmp){
			printk("tmp->name = %s\n",(*tmp)->name);

			//print_hex((*tmp)->value, (*tmp)->length);

			printk("*tmp->length = %d\n",(*tmp)->length);

			if(*(char *)((*tmp)->value + (*tmp)->length) == '\0'){
				printk("(*tmp)->value = %s\n",(*tmp)->value);
			}else 
				print_hex((*tmp)->value, (*tmp)->length);
			

			tmp = &((*tmp)->next);
		}
		else 
			break;
	}
}


void print_device_node_name(struct device_node *node)
{
	struct device_node **tmp = &node;

	while(1){
		if((*tmp)->child){
			
			printk("(*tmp)->child->name = %s\n",(*tmp)->child->name);
			
		}else {
		
			break;
		
		}

		tmp = &(*tmp)->child;
		
	}
	
	tmp = &node;

	while(1){
		if((*tmp)->parent){

			printk("(*tmp)->parent->name = %s\n",(*tmp)->parent->name);

		}else {
		
			break;
			
		}

		tmp = &(*tmp)->parent;
	}

	tmp = &node;

	while(1){
		if((*tmp)->sibling){

			printk("(*tmp)->sibling->name = %s\n",(*tmp)->sibling->name);

		}else {

			break;

		}

		tmp = &(*tmp)->sibling;
	}

	tmp = &node;

	while(1){
		if((*tmp)->next){

			printk("(*tmp)->next->name = %s\n",(*tmp)->next->name);

		} else {

			break;

		}

		tmp = &(*tmp)->next;

	}

		tmp = &node;

	while(1){
		if((*tmp)->allnext){

			printk("(*tmp)->allnext->name = %s\n",(*tmp)->allnext->name);
			print_property_name((*tmp)->allnext);

		} else {

			break;

		}

		tmp = &(*tmp)->allnext;
		printk("\n");
		printk("\n");
	}

}


static int __init dev_init(void)
{	
	int ret = misc_register(&misc1);
	printk("of_allnodes->name = %s\n",of_allnodes->name);

	print_device_node_name(of_allnodes);
	printk("misc_led initialed\n");
	return ret;

}


static void __exit dev_exit(void)
{
	misc_deregister(&misc1);
	printk("misc led unload\n");

}


module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
