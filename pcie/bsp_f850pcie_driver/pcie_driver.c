#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kfifo.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/msi.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/poll.h>


#define PCIE_DATA_SIMULATION    0 
#define TRANSFER_FIFO		0

struct pcie_f850_data{
	char *name;
	struct list_head dev_list_head;
	struct mutex main_lock;
	int endpoint_count;
};
struct pcie_f850_data *f850_pcie = NULL;

/* user space data interface */
struct pack_type {
	unsigned int data_head;         /* 0xaa55aa55 */
	unsigned int data_offset;
	unsigned int data_len;
	unsigned int type;
};

struct pcie_endpoint_data{
	char name[20];
	int node_index;
	int max_bar;

	struct pci_dev *pcie_dev;
	struct list_head list;
	resource_size_t  barStart[6];//bar _phyaddr
	resource_size_t barLen[6];//bar _len
	uint32_t *barVirt[6];//bar _ioremap

	wait_queue_head_t 		node_wait_que;
	struct pack_type 		pack_head;
	struct miscdevice 		misc_dev;
	struct file_operations 		misc_fops;
	unsigned int complete;		//wait_event condition

	struct kfifo 			mmap_read_fifo;
	spinlock_t 			mmap_read_fifo_lock;
	struct mutex 			read_fifo_lock;

	unsigned char 			*mmap_read_buf;
	unsigned char 			*mmap_write_buf;

	atomic_t			read_buf_lock;
	atomic_t			write_buf_lock;

#if PCIE_DATA_SIMULATION
	void                        *sim_package;
	struct task_struct          *data_producer;
#endif

};

struct reginfo{
	unsigned addr;
	unsigned int value;
};


struct fpga_data {
	unsigned int irq;
	uint32_t *base;
};

/**
 *Driver Version
 */
#define VERSION                     "V1.0.0"

#define PCIE_VENDOR               0x10ee
#define PCIE_DEVICE               0x7022

#define LOG_ERR		0
#define LOG_INFO	1
#define LOG_NOTICE	2
#define LOG_DEBUG	3
int fpga_log_level = LOG_DEBUG;

#define LOGPRT(lvl,x...)  do{ \
	if(lvl < (fpga_log_level + 1)) \
	printk(KERN_INFO "[PCIE ] " x); \
}while(0)


#define DATA_LEN 			1024
#define MMAP_BUF_READ_SIZE              SZ_4M   /* *mmap_read_buf;*/
#define MMAP_BUF_WRITE_SIZE             SZ_1M   /* *mmap_write_buf;*/
#define MMAP_BUF_READ_FIFO_SIZE    	SZ_4M


#define READ_STATE   _IOW('p', 0x01, int)
#define WRITE_STATE  _IOW('p', 0x02, int) 
#define SET_FPGA_REG _IOW('p', 0x03, int)
#define GET_FPGA_REG _IOW('p', 0x04, int)
#define WRITE_BUF_LOCK  	0
#define WRITE_BUF_SYNC		1
#define READ_BUF_LOCK		2


static uint32_t   bar_info = 0;
static uint32_t	  process_length = 0;
static uint32_t	  process_offset = 0;
static const struct file_operations *fops = NULL;





/*********************************************************/


unsigned int  write_read_offset = 0;

static ssize_t echo_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned int i;
	unsigned int count = 0;
	struct fpga_data *p_data = dev_get_drvdata(dev);
	void __iomem *base = p_data->base;

	LOGPRT(LOG_INFO,"base:0x%08x, offset:%d\n", (unsigned int )base, write_read_offset);

	for(i = 0; i < 0x10; i++){
		LOGPRT(LOG_INFO,"read var_addr:0x%08x, data:0x%08x\n", (unsigned int )((unsigned int )base + write_read_offset + (i * 4UL)), 
				readl(base + write_read_offset + (i * 4UL)));
	}

	return count;
}


static ssize_t echo_store_hex(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned int val;
	int error;
	struct fpga_data *p_data = dev_get_drvdata(dev);
	void __iomem *base = p_data->base;
	unsigned int i;
	unsigned int write_data;


	error = kstrtouint(buf, 16, &val);
	if (error)
		return error;

	LOGPRT(LOG_INFO,"base:0x%08x, offset:%d\n", (unsigned int )base, write_read_offset);


	for(i = 0; i < 0x10; i++)
	{
		write_data = val + i;
		writel(write_data, (base + write_read_offset + (i * 4UL)));
		LOGPRT(LOG_INFO,"write var_addr:0x%08x, data:0x%08x\n", (unsigned int )(((unsigned int )base + write_read_offset + (i * 4UL))), (write_data));
	}



	return count;
}
static ssize_t echo_store_offset(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned int val;
	int error;

	error = kstrtouint(buf, 16, &val);
	if (error)
	{
		return error;
	}

	write_read_offset = val;

	return count;
}

static ssize_t echo_show_offset(struct device *dev,
		struct device_attribute *attr, char *buf)

{
	char *s = buf;
	s += sprintf(s, "0x%08x\n", write_read_offset);

	return (s - buf);
}

static DEVICE_ATTR(matrix_read, 0664, echo_show, NULL);
static DEVICE_ATTR(matrix_write, 0664, NULL, echo_store_hex);
static DEVICE_ATTR(matrix_offset, 0664, echo_show_offset, echo_store_offset);

static struct attribute *mxt_attrs[] = {
	&dev_attr_matrix_read.attr,
	&dev_attr_matrix_write.attr,
	&dev_attr_matrix_offset.attr,
	NULL
};

static const struct attribute_group mxt_attr_group = {
	.attrs = mxt_attrs,
};

static int  mtx_sw_probe(struct platform_device *pdev)
{
	struct fpga_data *p_data;

	LOGPRT(LOG_INFO,"%s\n", __func__);
	dev_set_drvdata(&pdev->dev, pdev->dev.platform_data);
	p_data = dev_get_drvdata(&pdev->dev);

	if (sysfs_create_group(&pdev->dev.kobj, &mxt_attr_group))
	{
		LOGPRT(LOG_ERR, "%s:sysfs_create_group failed\n", __func__);        
	}

	return 0;
}

static int  mtx_sw_remove(struct platform_device *pdev)
{
	//struct fpga_data *p_data = dev_get_drvdata(&pdev->dev);

	LOGPRT(LOG_INFO,"%s\n",__func__) ;
	sysfs_remove_group(&pdev->dev.kobj, &mxt_attr_group);

	return 0;
}


static struct platform_driver f850_matrix_switch_driver = {
	.driver = {
		.name = "f850_main_fpga",
		.owner = THIS_MODULE,
	},
	.probe		= mtx_sw_probe,
	.remove		= mtx_sw_remove,
};

int  main_fpga_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&f850_matrix_switch_driver);
	if (ret)
	{
		printk("platform_driver_register failed\n");
	}

	return ret;
}

void  main_fpga_exit(void)
{
	LOGPRT(LOG_INFO,"%s\n",__func__);       
	platform_driver_unregister(&f850_matrix_switch_driver);
}



/*********************************************************/



static ssize_t log_level_show(struct device_driver *drv, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%d\n", fpga_log_level);

	return (s - buf);
}

static ssize_t log_level_store(struct device_driver *drv, const char *buf, size_t count)
{	
	unsigned int val;

	if (kstrtouint(buf, 10, &val))
		return -EINVAL;

	if (val < 0)
		return -EINVAL;

	fpga_log_level = val;

	return count;
}

static ssize_t driver_version_show(struct device_driver *drv, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%s\n", VERSION);

	return (s - buf);
}


static int found_attr_dev(struct device *dev)
{
	struct list_head *pos;
	struct pci_dev *pdev;
	struct pcie_endpoint_data *pcie_endpoint = NULL;
	unsigned int flag = 0;

	list_for_each(pos, &f850_pcie->dev_list_head)
	{
		pcie_endpoint = list_entry(pos, struct pcie_endpoint_data, list);
		pdev = container_of(dev, struct pci_dev, dev);
		LOGPRT(LOG_INFO,"name %s, %p, %p, pdev=%p,%p\n", pcie_endpoint->name, dev, &pcie_endpoint->pcie_dev->dev,
				pdev, pcie_endpoint->pcie_dev);
		if (dev == &pcie_endpoint->pcie_dev->dev)
		{
			flag = 1;
			break;
		}
	}

	return flag;
}

static ssize_t fpga_pcie_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	char *s = buf;
	struct pcie_endpoint_data *pcie_endpoint = NULL;    
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);

	if(found_attr_dev(dev)){
		uint32_t  i = 0;
		void __iomem * base_address = NULL;
		void __iomem * read_address = NULL;
		unsigned int read_data;
		pcie_endpoint = pci_get_drvdata(pdev); 

		if(bar_info >= pcie_endpoint->max_bar)
			bar_info = 0;

		base_address = pcie_endpoint->barVirt[bar_info];

		LOGPRT(LOG_INFO,"read bar:%d, base:0x%08x, offset:%d, length:%d\n",bar_info, (unsigned int )base_address, process_offset, process_length);

		for(i = 0; i < process_length; i++){
			read_address = base_address + process_offset + (i * 4UL);
			read_data = ioread32(read_address);

			s += sprintf(s, "read var_addr:0x%08x, data:0x%08x\n", (unsigned int )((unsigned int )base_address + process_offset + (i * 4UL)), read_data);
		}
	}else{
		s += sprintf(s, "cann't found pcie device\n");
	}

	return (s - buf);
}

static ssize_t fpga_pcie_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct pcie_endpoint_data *pcie_endpoint = NULL;
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);
	if(found_attr_dev(dev)){
		uint32_t  i = 0;
		void __iomem * base_address = NULL;
		void __iomem * write_address = NULL;
		unsigned int write_data;

		pcie_endpoint = pci_get_drvdata(pdev);
		if(bar_info >= pcie_endpoint->max_bar)
			bar_info = 0;

		base_address = pcie_endpoint->barVirt[bar_info];

		LOGPRT(LOG_INFO,"write bar:%d, base:0x%08x, offset:%d, length:%d, data: %s, count: %d\n",bar_info, (unsigned int )base_address, process_offset, process_length,
				buf, count);

		for(i = 0; i < process_length; i++){
			write_address = base_address + process_offset + (i * 4UL);
			write_data = buf[i];
			iowrite32(write_data, write_address);
			LOGPRT(LOG_INFO,"write var_addr:0x%08x, data:0x%08x\n", (unsigned int )((unsigned int )write_address), (write_data));
		}

	}

	return (count);
}

static ssize_t bar_num_show(struct device_driver *drv, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%d\n", bar_info);

	return (s - buf);
}

static ssize_t bar_num_store(struct device_driver *drv, const char *buf, size_t count)
{	
	unsigned int val;

	if (kstrtouint(buf, 16, &val))
		return -EINVAL;

	if (val < 0)
		return -EINVAL;

	bar_info = val;

	return count;
}


static ssize_t bar_offset_show(struct device_driver *drv, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%d\n", process_offset);

	return (s - buf);
}

static ssize_t bar_offset_store(struct device_driver *drv, const char *buf, size_t count)
{	
	unsigned int val;

	if (kstrtouint(buf, 16, &val))
		return -EINVAL;

	if (val < 0)
		return -EINVAL;

	process_offset = val;

	return count;
}


static ssize_t bar_data_length_show(struct device_driver *drv, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%d\n", process_length);

	return (s - buf);
}

static ssize_t bar_data_length_store(struct device_driver *drv, const char *buf, size_t count)
{	
	unsigned int val;

	if (kstrtouint(buf, 16, &val))
		return -EINVAL;

	if (val < 0)
		return -EINVAL;

	process_length = val;

	return count;
}



static DRIVER_ATTR_RO(driver_version);
static DRIVER_ATTR(log_level, 0664, log_level_show, log_level_store);

static DEVICE_ATTR(fpga_pcie, 0664, fpga_pcie_show, fpga_pcie_store);

static DRIVER_ATTR(bar_num, 0664, bar_num_show, bar_num_store);
static DRIVER_ATTR(bar_offset, 0664, bar_offset_show, bar_offset_store);
static DRIVER_ATTR(bar_data_length, 0664, bar_data_length_show, bar_data_length_store);


static struct attribute *mxt_attrs1[] = 
{
	&dev_attr_fpga_pcie.attr,
	NULL
};

static const struct attribute_group mxt_attr_group1 = {
	.attrs = mxt_attrs1,
};


static struct fpga_data fpga_misc_data ;

/*
   void dummy_dev_release(struct device *dev)
   {
   return;
   }

   static struct platform_device main_fpga_device = {
   .name = "f850_main_fpga",
   .id	= 0,
   .dev = {
   .platform_data = &fpga_misc_data,
   .release = &dummy_dev_release,
   },
   };
 */
//static struct platform_device *devices[] __initdata = {
//	&main_fpga_device,
//};

void BAR_request(struct pci_dev *dev){
	unsigned int i, ret;
	struct pcie_endpoint_data *pcie_endpoint = pci_get_drvdata(dev); 
	//LOGPRT(LOG_INFO, "MEM: %x IO: %x\n", pci_select_bars(dev, IORESOURCE_MEM), pci_select_bars(dev, IORESOURCE_IO));
	pcie_endpoint->max_bar = pci_select_bars(dev, IORESOURCE_MEM)-1;

	for(i=0; i<pcie_endpoint->max_bar; i++){
		pcie_endpoint->barLen[i]  = pci_resource_len(dev, i);
		pcie_endpoint->barStart[i] = pci_resource_start (dev, i);
		ret = pci_request_region(dev, i, "F850_fpga_bar");
		LOGPRT(LOG_INFO,"mem region %d %s\n",i, (!ret)? "OK":"ERR");
		pcie_endpoint->barVirt[i] = pci_iomap(dev, i, pci_resource_len(dev, i));
		LOGPRT(LOG_INFO,"bar %d len 0x%x, phy 0x%08x, vir 0x%p\n", i, pcie_endpoint->barLen[0],
				pcie_endpoint->barStart[i],
				pcie_endpoint->barVirt[i]);
	}
}

void PCI_setMaster(struct pci_dev *dev)
{
	uint16_t cmdVal;

	/* set the DMA mask */
	if (pci_set_dma_mask(dev, DMA_BIT_MASK(32))) {
	}
#if 1
	pci_set_master(dev);
	pci_read_config_word(dev, PCI_COMMAND, (u16 *)&cmdVal);
	//LOGPRT(LOG_INFO,"read 1 PCI_COMMAND = 0x%04x\n",cmdVal);
	/* and set the master bit in command register. */
	cmdVal |= (	PCI_COMMAND_MEMORY
			| PCI_COMMAND_IO
			| PCI_COMMAND_MASTER);

	/* and clear the interrupt disable bit in command register. */
	cmdVal &= ~PCI_COMMAND_INTX_DISABLE;
	//LOGPRT(LOG_INFO,"write PCI_COMMAND = 0x%04x\n",cmdVal);
	pci_write_config_word(dev, PCI_COMMAND, cmdVal);

	pci_read_config_word(dev, PCI_COMMAND, (u16 *)&cmdVal);
	//LOGPRT(LOG_INFO,"read 2 PCI_COMMAND = 0x%04x\n",cmdVal);
#endif	
}

void PCI_freeBAR(struct pci_dev *dev)
{
	struct pcie_endpoint_data *pcie_endpoint = pci_get_drvdata(dev); 
	iounmap(pcie_endpoint->barVirt[0]);
	iounmap(pcie_endpoint->barVirt[1]);
	pci_release_region(dev, 0);
	pci_release_region(dev, 1);
}

struct pcie_endpoint_data *alloc_add_list(struct pci_dev *dev)
{
	struct pcie_endpoint_data *pcie_endpoint;

	pcie_endpoint = kzalloc(sizeof(struct pcie_endpoint_data), GFP_KERNEL);

	pcie_endpoint->pcie_dev = dev;
	list_add_tail(&pcie_endpoint->list, &f850_pcie->dev_list_head);
	f850_pcie->endpoint_count++;
	pcie_endpoint->node_index = f850_pcie->endpoint_count;
	sprintf(pcie_endpoint->name, "pcie_endpoint%d", pcie_endpoint->node_index);
	LOGPRT(LOG_INFO,"list_add device: name=%s\n", pci_name(dev));

	return pcie_endpoint;
}

void free_remove_list(struct pci_dev *dev)
{
	struct list_head *n, *pos;
	struct pcie_endpoint_data *pcie_endpoint2;
	struct pcie_endpoint_data *pcie_endpoint = pci_get_drvdata(dev); 

	list_for_each_safe(pos, n, &f850_pcie->dev_list_head)
	{
		pcie_endpoint2 = list_entry(pos, struct pcie_endpoint_data, list);
		if (pcie_endpoint2 == pcie_endpoint)
		{
			LOGPRT(LOG_INFO,"list_remove device: name=%s\n", pci_name(dev));
			list_del(pos);
			kfree(pcie_endpoint2);
			return;
		}
	}
}

#if PCIE_DATA_SIMULATION
static int pcie_data_simulation_producer(void *data)
{
	struct pcie_endpoint_data *pcie_endpoint = (struct pcie_endpoint_data *)data;

	while (!kthread_should_stop()) {

		pcie_endpoint->pack_head.data_head 		=  0x08;
		pcie_endpoint->pack_head.data_len 		=  DATA_LEN;
		pcie_endpoint->pack_head.type 			=  0x000106da;
		pcie_endpoint->pack_head.data_offset 		=  0;

		kfifo_in_locked(&pcie_endpoint->mmap_read_fifo, (unsigned char *)&pcie_endpoint->pack_head,
				sizeof(struct pack_type), &pcie_endpoint->mmap_read_fifo_lock);
		if (pcie_endpoint->pack_head.data_len != 0)
		{

			kfifo_in_locked(&pcie_endpoint->mmap_read_fifo, (unsigned char *)pcie_endpoint->sim_package,
					pcie_endpoint->pack_head.data_len, &pcie_endpoint->mmap_read_fifo_lock);
		}
		atomic_inc(&pcie_endpoint->read_buf_lock);

		wake_up(&pcie_endpoint->node_wait_que);
		msleep(10);
	}

	return 0;
}
#endif

static ssize_t f850_open(struct inode *inode, struct file * filp)
{

	struct pcie_endpoint_data *pcie_endpoint =
			container_of(filp->f_op, struct pcie_endpoint_data, misc_fops);

	filp->private_data = pcie_endpoint;
#if PCIE_DATA_SIMULATION
	pcie_endpoint->data_producer = kthread_run(pcie_data_simulation_producer, pcie_endpoint, "SubBoard%d_producer", pcie_endpoint->node_index);

#endif

	return 0;
}

static ssize_t f850_release(struct inode * inode, struct file * filp)
{
	struct pcie_endpoint_data *pcie_endpoint = filp->private_data;
#if PCIE_DATA_SIMULATION
	kthread_stop(pcie_endpoint->data_producer);
#endif

	return 0;
}

static ssize_t f850_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long bus_address;
	int error = 0;
	struct pcie_endpoint_data *pcie_endpoint = filp->private_data;
	//printk("f850_mmap %p, read %p write %p\n", filp->private_data, pcie_endpoint->mmap_read_buf, pcie_endpoint->mmap_write_buf);
	
	if (vma->vm_pgoff == 0)
	{
		/* Get physical address */
		bus_address = virt_to_phys(pcie_endpoint->mmap_read_buf);
	} else {
		/* Get physical address */
		bus_address = virt_to_phys(pcie_endpoint->mmap_write_buf);
	}

	if (remap_pfn_range(vma, vma->vm_start, (bus_address >> PAGE_SHIFT),
				vma->vm_end - vma->vm_start, vma->vm_page_prot))
	{

		error = -EAGAIN;
		goto end_function;
	}
	//LOGPRT(LOG_INFO, "%s, vm_end %lx, vm_start %lx, vma->vm_pgoff %lx, bus_addr %lx\n",
	//		__func__, vma->vm_end, vma->vm_start, vma->vm_pgoff, bus_address);

end_function:
	return error;
}

unsigned int f850_poll(struct file *filp, poll_table *wait)
{
	struct pcie_endpoint_data *pcie_endpoint = filp->private_data;

	poll_wait(filp, &pcie_endpoint->node_wait_que, wait);
	if (!!kfifo_len(&pcie_endpoint->mmap_read_fifo))
	{
		return POLLIN | POLLRDNORM;
	}

	return 0;
}

static int get_fifo(struct pcie_endpoint_data *pcie_endpoint)
{
	int ret = 0;
	struct pack_type *head_p;
	mutex_lock(&pcie_endpoint->read_fifo_lock);
	ret = atomic_read(&pcie_endpoint->read_buf_lock);
	if (ret)
	{
		kfifo_out_locked(&pcie_endpoint->mmap_read_fifo, pcie_endpoint->mmap_read_buf, 
				sizeof(struct pack_type), &pcie_endpoint->mmap_read_fifo_lock);
		head_p = (struct pack_type *)((unsigned char *)pcie_endpoint->mmap_read_buf);
		kfifo_out_locked(&pcie_endpoint->mmap_read_fifo,
				pcie_endpoint->mmap_read_buf + sizeof(struct pack_type), 
				head_p->data_len, &pcie_endpoint->mmap_read_fifo_lock);
		atomic_dec(&pcie_endpoint->read_buf_lock);
		ret = 1;
		LOGPRT(LOG_DEBUG,"%s: node_index %d, read OK packet remain %d, read_len = %d, fifo_avail %d, fifo_used %d\n",
				__func__, 
				pcie_endpoint->node_index,
				atomic_read(&pcie_endpoint->read_buf_lock),
				head_p->data_len + 16,
				kfifo_avail(&pcie_endpoint->mmap_read_fifo),
				kfifo_len(&pcie_endpoint->mmap_read_fifo)
		      );
	}
	mutex_unlock(&pcie_endpoint->read_fifo_lock);
	return ret;
}


static int put_fifo(struct pcie_endpoint_data *pcie_endpoint)
{
	int err = 0;
	unsigned int count = 10;
	struct pack_type *head_p;

	while ((kfifo_avail(&pcie_endpoint->mmap_read_fifo) < pcie_endpoint->pack_head.data_len + sizeof(struct pack_type))
			&& (count != 0))
	{
		mutex_lock(&pcie_endpoint->read_fifo_lock);
		LOGPRT(LOG_DEBUG,"[DELECT ] node_index %d >>> need delete 1 packet data  kfifo_avail %d, data need %d\n",
				pcie_endpoint->node_index,
				kfifo_avail(&pcie_endpoint->mmap_read_fifo), 
				pcie_endpoint->pack_head.data_len + sizeof(struct pack_type));

		kfifo_out_locked(&pcie_endpoint->mmap_read_fifo, pcie_endpoint->mmap_read_buf, sizeof(struct pack_type),
				&pcie_endpoint->mmap_read_fifo_lock);
		head_p = (struct pack_type *)((unsigned char *)pcie_endpoint->mmap_read_buf);
		if (head_p->data_len == 0)
		{
			kfifo_in_locked(&pcie_endpoint->mmap_read_fifo, pcie_endpoint->mmap_read_buf, 
					sizeof(struct pack_type), &pcie_endpoint->mmap_read_fifo_lock);
			count--;
			LOGPRT(LOG_DEBUG,"[DELECT ] node_index %d >>> retry %d can not datect this packet\n", pcie_endpoint->node_index, count); 
		} else {
			kfifo_out_locked(&pcie_endpoint->mmap_read_fifo, pcie_endpoint->mmap_read_buf + sizeof(struct pack_type),
					head_p->data_len, &pcie_endpoint->mmap_read_fifo_lock);
			atomic_dec(&pcie_endpoint->read_buf_lock);
			LOGPRT(LOG_DEBUG,"[DELECT ] node_index %d >>> OK  kfifo_avail %d, type %08x\n", 
					pcie_endpoint->node_index, kfifo_avail(&pcie_endpoint->mmap_read_fifo), head_p->type); 
		}
		mutex_unlock(&pcie_endpoint->read_fifo_lock);
	}
	if (!count)
	{
		LOGPRT(LOG_ERR,"[ERR ]%s node_index %d >>> retry timeout kfifo_avail %d,"
				" data need %d skip fifo_in and delect this packet return\n",
				__func__, pcie_endpoint->node_index, kfifo_avail(&pcie_endpoint->mmap_read_fifo),
				pcie_endpoint->pack_head.data_len + sizeof(struct pack_type) ); 
		return -1;
	}

	kfifo_in_locked(&pcie_endpoint->mmap_read_fifo, (unsigned char *)&pcie_endpoint->pack_head,
			sizeof(struct pack_type), &pcie_endpoint->mmap_read_fifo_lock);
	if (pcie_endpoint->pack_head.data_len != 0)
	{
		kfifo_in_locked(&pcie_endpoint->mmap_read_fifo, (unsigned char *)pcie_endpoint->mmap_write_buf,
				pcie_endpoint->pack_head.data_len, &pcie_endpoint->mmap_read_fifo_lock);
	}
	atomic_inc(&pcie_endpoint->read_buf_lock);

	LOGPRT(LOG_DEBUG,"%s: node_index %d put packet %d to fifo; head=0x%08x,"
			" type=0x%08x, len=%d, fifo_used %d, fifo_avail %d\n", __func__, 
			pcie_endpoint->node_index,
			atomic_read(&pcie_endpoint->read_buf_lock),
			pcie_endpoint->pack_head.data_head,
			pcie_endpoint->pack_head.type,
			pcie_endpoint->pack_head.data_len,
			kfifo_len(&pcie_endpoint->mmap_read_fifo),
			kfifo_avail(&pcie_endpoint->mmap_read_fifo)
	      );

	wake_up(&pcie_endpoint->node_wait_que);

	return err;
}


int process_fpga_reg(unsigned int cmd, struct reginfo *reg, struct pcie_endpoint_data *pcie_endpoint)
{
	uint32_t val;
	int ret=0;
	void __iomem *base_address = NULL;
	void __iomem *reg_address = NULL;


	base_address = pcie_endpoint->barVirt[1];
	reg_address = base_address + reg->addr;	

	switch(cmd){
		case SET_FPGA_REG:
			iowrite32(reg->value, reg_address);
			break;
		case GET_FPGA_REG:
			reg->value = ioread32(reg_address);
			break;
		default:
			ret = -1;
	}

	return ret;
}

static int put_fpga_dev_bar(struct pcie_endpoint_data *pcie_endpoint)
{
	uint32_t  i = 0;
	void __iomem * base_address = NULL;
	void __iomem * write_address = NULL;
	unsigned int write_data;

	struct pack_type *ptype = &pcie_endpoint->pack_head;
	char *buf = pcie_endpoint->mmap_write_buf;

	if(bar_info >= pcie_endpoint->max_bar)
		bar_info = 0;

	base_address = pcie_endpoint->barVirt[bar_info];

	//LOGPRT(LOG_INFO,"buf %p write bar:%d, base:0x%08x, offset:%d data_len %d\n", buf, bar_info, (unsigned int )base_address, process_offset, ptype->data_len+sizeof(struct pack_type)); 
	for(i = 0; i < ptype->data_len+sizeof(struct pack_type); i++){
		write_address = base_address + process_offset + (i * 4UL);
		write_data = buf[i];
		iowrite32(write_data, write_address);
	}

	return 1;
}

static int get_fpga_dev_bar(struct pcie_endpoint_data *pcie_endpoint)
{
	uint32_t  i = 0, j = 0;
	void __iomem * base_address = NULL;
	void __iomem * read_address = NULL;
	unsigned int read_data;
	struct pack_type *ptype;

	if(bar_info >= pcie_endpoint->max_bar)
		bar_info = 0;

	base_address = pcie_endpoint->barVirt[bar_info];

	//LOGPRT(LOG_INFO,"buf %p read bar:%d, base:0x%08x, offset:%d\n",pcie_endpoint->mmap_read_buf, bar_info, (unsigned int )base_address, process_offset);

	for(i=0; i<sizeof(struct pack_type); i++){
		read_address = base_address + process_offset + (i * 4UL);
		read_data = ioread32(read_address);
		pcie_endpoint->mmap_read_buf[i] = read_data;
	}

	ptype = (struct pack_type *)pcie_endpoint->mmap_read_buf;
	for(j=0; j < ptype->data_len; j++){
		read_address = base_address + process_offset + ((i+j) * 4UL);
		read_data = ioread32(read_address);
		pcie_endpoint->mmap_read_buf[i+j] = read_data;
	}
	
	return 1;
}

static long f850_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct reginfo reg;
	union {
		struct reginfo __user *reg;
		int __user *arg;
	}ureg;
	ureg.arg = (int __user *)arg;

	struct pcie_endpoint_data *pcie_endpoint = filp->private_data;

	//LOGPRT(LOG_INFO, "f850_ioctl: cmd %08x arg %d, pcie %s\n", cmd, arg, pcie_endpoint->name);
	switch(cmd){
		case WRITE_STATE:
			switch(arg){
				case WRITE_BUF_LOCK:
					if (0 == atomic_read(&pcie_endpoint->write_buf_lock))
					{	
						atomic_inc(&pcie_endpoint->write_buf_lock);
						ret = 1;
					}
					break;					
				case WRITE_BUF_SYNC:
					memcpy((unsigned char *)&pcie_endpoint->pack_head, 
							pcie_endpoint->mmap_write_buf, 
							sizeof(struct pack_type));
					struct pack_type *pt = (struct pack_type *)pcie_endpoint->mmap_write_buf;
                                        printk("phead: head: %x, offset: %x, len: %x, size: %d\n", pt->data_head, pt->data_offset, pt->data_len,
                                                                                                                     sizeof(struct pack_type));
#if  TRANSFER_FIFO
					//ret = put_fifo(pcie_endpoint);
#else
					ret = put_fpga_dev_bar(pcie_endpoint);	
#endif
					atomic_set(&pcie_endpoint->write_buf_lock, 0);
					wake_up(&pcie_endpoint->node_wait_que);
					break;
				default:
					LOGPRT(LOG_INFO, "%s: node_index %d, unknow write arg\n", __func__, pcie_endpoint->node_index);		
			};
			break;		
		case READ_STATE:
			switch(arg){
				case READ_BUF_LOCK:
#if  TRANSFER_FIFO
					//ret = get_fifo(pcie_endpoint);
#else
					ret = get_fpga_dev_bar(pcie_endpoint);
#endif
					break;
				default:
					LOGPRT(LOG_INFO, "%s: node_index %d, unknow read cmd arg\n", __func__, pcie_endpoint->node_index);
			};
			break;
		case SET_FPGA_REG:
		case GET_FPGA_REG:
			if(copy_from_user(&reg, ureg.reg, sizeof(struct reginfo)))
			{
				ret = -1;
				LOGPRT(LOG_INFO, "%s: slot %s cmd %d copy_from_user reg failed\n", __func__, pcie_endpoint->node_index, cmd);
				break;
			}

			if(ret = process_fpga_reg(cmd, &reg, pcie_endpoint))
			{
				ret = -2;
				LOGPRT(LOG_INFO, "%s: node_index %d, unknow read cmd arg\n", __func__, pcie_endpoint->node_index);
			}
			
                        if(copy_to_user(ureg.reg, &reg, sizeof(struct reginfo)))
                        {
                                ret = -3;
                                LOGPRT(LOG_INFO, "%s: slot %s cmd %d copy_to_user reg failed\n", __func__, pcie_endpoint->node_index, cmd);
                                break;
                        }
				
			break;
		default:
			LOGPRT(LOG_INFO, "%s: node_index %d, unknow cmd\n", __func__, pcie_endpoint->node_index);
	};

	return ret;
}

static ssize_t f850_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t f850_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	return 0;
}


static const struct file_operations f850_fops =
{
        .owner   = THIS_MODULE,
        .open    = f850_open,
        .release = f850_release,
        .poll    = f850_poll,
        .mmap    = f850_mmap,
        .read    = f850_read,
        .write   = f850_write,
        .unlocked_ioctl = f850_ioctl,
};


void init_ep_misc(struct pci_dev *dev)
{
	int err;
	unsigned long page;
	struct pcie_endpoint_data *pcie_endpoint = pci_get_drvdata(dev);
	struct miscdevice *mdev = &pcie_endpoint->misc_dev;
	
	//add misc device and driver
	init_waitqueue_head(&pcie_endpoint->node_wait_que);
	pcie_endpoint->misc_dev.minor	= 255;
	pcie_endpoint->misc_dev.name	= pcie_endpoint->name;

	pcie_endpoint->misc_fops.owner = THIS_MODULE;
	pcie_endpoint->misc_fops.open  = f850_open;
	pcie_endpoint->misc_fops.release = f850_release;
	pcie_endpoint->misc_fops.mmap = f850_mmap;
	pcie_endpoint->misc_fops.unlocked_ioctl = f850_ioctl;

	fops = pcie_endpoint->misc_dev.fops	= &pcie_endpoint->misc_fops;
	
	
	err = misc_register(&pcie_endpoint->misc_dev);
	if (err)
	{
		LOGPRT(LOG_ERR,  "[ERR ] misc_register\n");
	}

	pcie_endpoint->mmap_read_buf = (char *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, get_order(MMAP_BUF_READ_SIZE));

	for(page=(unsigned long)pcie_endpoint->mmap_read_buf; page<MMAP_BUF_READ_SIZE; page += PAGE_SIZE){
		SetPageReserved(virt_to_page(page));
	}

#if PCIE_DATA_SIMULATION
	pcie_endpoint->sim_package = (char *)kzalloc(MMAP_BUF_READ_SIZE, 0x00);
	if (pcie_endpoint->sim_package == NULL) {
		LOGPRT(LOG_ERR,  "[ERR ] sim_package alloc\n");
	}
#endif
	spin_lock_init(&pcie_endpoint->mmap_read_fifo_lock);
	err = kfifo_alloc(&pcie_endpoint->mmap_read_fifo,  MMAP_BUF_READ_FIFO_SIZE, GFP_KERNEL);
	if (err)
	{
		LOGPRT(LOG_ERR,  "[ERR ] kfifo_alloc\n");
	}

	pcie_endpoint->mmap_write_buf = (char *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, get_order(MMAP_BUF_WRITE_SIZE));

        for(page=(unsigned long)pcie_endpoint->mmap_write_buf; page<MMAP_BUF_WRITE_SIZE; page += PAGE_SIZE){
                SetPageReserved(virt_to_page(page));
        }

	printk("read_buf_size %x %p write_buf_size %x %p\n", MMAP_BUF_READ_SIZE, pcie_endpoint->mmap_read_buf, MMAP_BUF_WRITE_SIZE, pcie_endpoint->mmap_write_buf);
}

static int pcie_drv_probe(struct pci_dev *dev,
		const struct pci_device_id *id)
{
	int err;
	struct pcie_endpoint_data *pcie_endpoint;
	//LOGPRT(LOG_INFO,"%s\n", __func__);
	pcie_endpoint = alloc_add_list(dev);
	pci_set_drvdata(dev, pcie_endpoint);

	err = pci_enable_device(dev);
	if(err){
		LOGPRT(LOG_ERR,  "pci_enable_device failed\n");
		return err;
	}

	PCI_setMaster(dev);
	BAR_request(dev);
#if 1
	err = pci_enable_msi(dev);
	if(err)
		LOGPRT(LOG_ERR,  "pci_enable_msi failed\n");
#endif
	//LOGPRT(LOG_INFO,"%s,%d,   %d\n", __func__,dev->irq, err);


	if (sysfs_create_group(&dev->dev.kobj, &mxt_attr_group1))
	{
		LOGPRT(LOG_ERR, "%s:sysfs_create_group failed\n", __func__);
	} 

	fpga_misc_data.irq = dev->irq;
	fpga_misc_data.base = pcie_endpoint->barVirt[1];


	mutex_init(&pcie_endpoint->read_fifo_lock);
	init_ep_misc(dev);	

	//platform_add_devices(devices, ARRAY_SIZE(devices));
	//main_fpga_init();
	return 0;
}

static void pcie_drv_remove(struct pci_dev *dev)
{
	struct pcie_endpoint_data *pcie_endpoint = pci_get_drvdata(dev); 
	LOGPRT(LOG_INFO,"%s: %s\n", __func__, pcie_endpoint->name);

	//main_fpga_exit();
	//platform_device_unregister(&main_fpga_device);


	sysfs_remove_group(&dev->dev.kobj, &mxt_attr_group1);

	mutex_lock(&f850_pcie->main_lock);
	pci_disable_msi(dev);
        free_pages((unsigned long)pcie_endpoint->mmap_write_buf, get_order(MMAP_BUF_WRITE_SIZE));
        kfifo_free(&pcie_endpoint->mmap_read_fifo);
        free_pages((unsigned long)pcie_endpoint->mmap_read_buf, get_order(MMAP_BUF_READ_SIZE));
        misc_deregister(&pcie_endpoint->misc_dev);

	pci_disable_device(dev);
	PCI_freeBAR(dev);
	free_remove_list(dev);
	mutex_unlock(&f850_pcie->main_lock);
}

static const struct pci_device_id port_pci_ids[] = { 
	{
		/* handle any PCI-Express port */
		.vendor 	= PCIE_VENDOR,
		.device 	= PCIE_DEVICE,
		.subvendor	= PCI_ANY_ID,
		.subdevice	= PCI_ANY_ID,
	}, 
	{ 
		0 /* end: all zeroes */ 
	},
};
MODULE_DEVICE_TABLE(pci, port_pci_ids);

static struct pci_driver pcie_portdriver = {
	.name		= "F850_mainfpga",
	.id_table	= port_pci_ids,
	.probe		= pcie_drv_probe,
	.remove		= pcie_drv_remove,

};

void f850_pcie_init(void)
{
	f850_pcie = kzalloc(sizeof(struct pcie_f850_data), GFP_KERNEL);
	f850_pcie->name = "f850_x86_pcie";
	f850_pcie->endpoint_count = 0;
	INIT_LIST_HEAD(&f850_pcie->dev_list_head);
	mutex_init(&f850_pcie->main_lock);

	//LOGPRT(LOG_INFO,"f850_pcie name: %s\n", f850_pcie->name);
}

void f850_pcie_free(void)
{
	kfree(f850_pcie);
	f850_pcie=NULL;
}

static int __init F850_misc_init(void)
{
	int ret = 0;

	LOGPRT(LOG_INFO,"init f850 pci driver: f850_pcie\n");
	f850_pcie_init();
	//LOGPRT(LOG_INFO,"register the pci driver:%s\n", __func__);
	ret = pci_register_driver(&pcie_portdriver);
	if(ret)
		LOGPRT(LOG_INFO, "register pci drvier %s failed\n", pcie_portdriver.name);	

	if (driver_create_file(&pcie_portdriver.driver, &driver_attr_log_level))
	{
		LOGPRT(LOG_ERR, "%s:driver_create_file %s failed\n", __func__, driver_attr_log_level.attr.name);        
	}

	if (driver_create_file(&pcie_portdriver.driver, &driver_attr_driver_version))
	{
		LOGPRT(LOG_ERR, "%s:sysfs_create_group failed\n", __func__);        
	}

	if (driver_create_file(&pcie_portdriver.driver, &driver_attr_bar_num))
	{
		LOGPRT(LOG_ERR, "%s:driver_create_file %s failed\n", __func__, driver_attr_bar_num.attr.name);        
	}

	if (driver_create_file(&pcie_portdriver.driver, &driver_attr_bar_offset))
	{
		LOGPRT(LOG_ERR, "%s:driver_create_file %s failed\n", __func__, driver_attr_bar_offset.attr.name);        
	}

	if (driver_create_file(&pcie_portdriver.driver, &driver_attr_bar_data_length))
	{
		LOGPRT(LOG_ERR, "%s:driver_create_file %s failed\n", __func__, driver_attr_bar_data_length.attr.name);        
	}	

	//LOGPRT(LOG_INFO,"register the pci driver complete :%s ret:%d\n", __func__, ret);


	return ret;
}

static void __exit F850_misc_exit(void)
{       
	driver_remove_file(&pcie_portdriver.driver, &driver_attr_bar_data_length);
	driver_remove_file(&pcie_portdriver.driver, &driver_attr_bar_offset);
	driver_remove_file(&pcie_portdriver.driver, &driver_attr_bar_num);
	driver_remove_file(&pcie_portdriver.driver, &driver_attr_driver_version);	
	driver_remove_file(&pcie_portdriver.driver, &driver_attr_log_level);
	pci_unregister_driver(&pcie_portdriver);
	f850_pcie_free();
}

module_init(F850_misc_init);
module_exit(F850_misc_exit);
MODULE_DESCRIPTION("F850 front pannel driver, version 1.1");
MODULE_LICENSE("GPL");

