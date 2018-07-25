#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/of_gpio.h>



#if 0
struct property {
	char	*name;
	int	length;
	void	*value;
	struct property *next;
	unsigned long _flags;
	unsigned int unique_id;
};

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
#endif

extern struct device_node *of_allnodes;
extern struct device_node *of_chosen;
extern struct device_node *of_aliases;


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

	while(1){
		if((*tmp)->sibling){

			printk("(*tmp)->sibling->name = %s\n",(*tmp)->sibling->name);

		}else {

			break;

		}

		tmp = &(*tmp)->sibling;
	}

	while(1){
		if((*tmp)->next){

			printk("(*tmp)->next->name = %s\n",(*tmp)->next->name);

		} else {

			break;

		}

		tmp = &(*tmp)->next;

	}

	while(1){
		if((*tmp)->allnext){

			printk("(*tmp)->allnext->name = %s\n",(*tmp)->allnext->name);

		} else {

			break;

		}

		tmp = &(*tmp)->allnext;
	}

}



static int  __init dtb_init(void )
{
	print_device_node_name(of_allnodes);

	return 0;
	
}

static int  __exit dtb_exit(void)
{
	return 0;

}

module_init(dtb_init);
module_exit(dtb_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WYF");
