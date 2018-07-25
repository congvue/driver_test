/* 
Function description:When we call dmatest_read(),it will transmit src memory data 
to dst memory,then print dst memory data by dma_callback_func(void) function. 
*/  
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/pwm.h>
#include <linux/of.h>
#include <linux/of_device.h> 

#if 0

struct clk {
	struct clk_core	*core;
	const char *dev_id;
	const char *con_id;
	unsigned long min_rate;
	unsigned long max_rate;
	struct hlist_node clks_node;
};


struct clk_core {
	const char		*name;
	const struct clk_ops	*ops;
	struct clk_hw		*hw;
	struct module		*owner;
	struct clk_core		*parent;
	const char		**parent_names;
	struct clk_core		**parents;
	u8			num_parents;
	u8			new_parent_index;
	unsigned long		rate;
	unsigned long		req_rate;
	unsigned long		new_rate;
	struct clk_core		*new_parent;
	struct clk_core		*new_child;
	unsigned long		flags;
	unsigned int		enable_count;
	unsigned int		prepare_count;
	unsigned long		accuracy;
	int			phase;
	struct hlist_head	children;
	struct hlist_node	child_node;
	struct hlist_node	debug_node;
	struct hlist_head	clks;
	unsigned int		notifier_count;
#ifdef CONFIG_DEBUG_FS
	struct dentry		*dentry;
#endif
	struct kref		ref;
};
  
#endif

static const struct of_device_id imx_pwm_dt_ids[] = {
	{ .compatible = "fsl,imx1-pwm"},
	{ .compatible = "fsl,imx27-pwm"},
	{ /* sentinel */ }
};


static int imx_pwm_probe(struct platform_device *pdev)
{
	const struct of_device_id *of_id = 
						of_match_device(imx_pwm_dt_ids,&pdev->dev);

	printk("pdev->dev->name  = %s\n",__func__);
	

}

static int imx_pwm_remove(struct platform_device *dev)
{
	return 0;

}



static struct platform_driver imx_pwm_driver = {
	.driver		= {
		.name	= "imx-pwm",
		.of_match_table = imx_pwm_dt_ids,
	},
	.probe		= imx_pwm_probe,
	.remove		= imx_pwm_remove,
};

module_platform_driver(imx_pwm_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Sascha Hauer <s.hauer@pengutronix.de>");

