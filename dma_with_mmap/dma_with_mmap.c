//这是测试frambuffer的mmap()函数的驱动程序

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/ctype.h>
#include <linux/pagemap.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>

#include<linux/dmaengine.h>  
#include<linux/dma-mapping.h>  
 
#include "demo.h"


struct simple_dev *simple_device;


#define SIZE  1024 *5


struct dma_chan *chan;  //DMA通道申请
struct dma_device *dev;  //DMA CLIENT
struct dma_async_tx_descriptor *tx = NULL;  //DMA 描述符
enum dma_ctrl_flags flags;   //DMA控制符
dma_cookie_t cookie; 

 
 dma_addr_t dma_src;  
 dma_addr_t dma_dst;  
 //virtual address	
 char *src = NULL;	
 char *dst = NULL ;


int simple_open(struct inode *inode, struct file *filp)
{
	struct simple_dev *dev;
	dev = container_of(inode->i_cdev, struct simple_dev, cdev);

	filp->private_data = dev;

	return 0;

}

int simple_release(struct inode *inode, struct file *filp)
{
	return 0;	
}

static int simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	
	ret = remap_pfn_range(vma, vma->vm_start,dma_dst>>PAGE_SHIFT,
		vma->vm_end - vma->vm_start, PAGE_SHARED);

	if(ret != 0) {
		return -EAGAIN;
	}

	return 0;
	
}

void dma_callback_func(void)  
{  
    int i;  
	printk("this is dma_callback_fun\n");
} 


 ssize_t simple_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos) 
{
	 int ret = 0;
	  int i;
	  //alloc a desc,and set dst_addr,src_addr,data_size. 
	  memset(src, 0, SIZE);
	  copy_from_user(src, buf, size);
	    printk("src data\n");
	  for (i = 0; i < 20; i++){  
	 		printk("%c",src[i]);  
	  } 
	 tx = dev->device_prep_dma_memcpy(chan, dma_dst,dma_src, SIZE, DMA_COMPL_SKIP_DEST_UNMAP);  


	  if (!tx){  
		  printk(KERN_INFO "Failed to prepare DMA memcpy");  
	  }  
	  tx->callback = dma_callback_func;//set call back function  
	  tx->callback_param = NULL;  
	  cookie = tx->tx_submit(tx); //submit the desc  
	  if (dma_submit_error(cookie)){  
		  printk(KERN_INFO "Failed to do DMA tx_submit");  
	  }  

	  printk("dst data\n");

	  dma_async_issue_pending(chan);//begin dma transfer  
	
	  
	  for (i = 0; i < 20; i++){  
		  printk("%c",dst[i]);  
	  } 
	  printk("\n");
	
	  return ret;  

}

ssize_t simple_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)   
{  
    int ret = 0;
	int i;
    //alloc a desc,and set dst_addr,src_addr,data_size.  
    tx = dev->device_prep_dma_memcpy(chan, dma_dst, dma_src, SIZE, flags);  
    if (!tx){  
        printk(KERN_INFO "Failed to prepare DMA memcpy");  
    }  
      
    tx->callback = dma_callback_func;//set call back function  
    tx->callback_param = NULL;  
    cookie = tx->tx_submit(tx); //submit the desc  
    if (dma_submit_error(cookie)){  
        printk(KERN_INFO "Failed to do DMA tx_submit");  
    }  

     for (i = 0; i < 20; i++){  
        printk(KERN_INFO "%c",dst[i]);  
    } 
	
	printk("begin dma transfer\n");
    dma_async_issue_pending(chan);//begin dma transfer  

	printk("after dma transfer\n");

    for (i = 0; i < 20; i++){  
        printk(KERN_INFO "%c",dst[i]);  
    } 
		printk("22after dma transfer\n");
      
    return ret;  


}  


struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.open = simple_open, 
	.mmap = simple_mmap,
	.write = simple_write,
	.read = simple_read,
	.release = simple_release,
};

void simple_cleanup_module(void)
{
	dev_t devno = MKDEV(simple_MAJOR, simple_MINOR);
	unsigned long virt_addr;

	if(simple_device){
		cdev_del(&simple_device->cdev);
		kfree(simple_device);
	}


	unregister_chrdev_region(devno, 1);
	

}




int simple_init_module(void)
{
	int devnum = register_chrdev(0,"dma_with_mmap",&simple_fops);  

	struct class* my_class = class_create(THIS_MODULE, "dma_with_mmap");
	
	device_create(my_class, NULL,MKDEV(devnum,0), NULL,"dma_with_mmap");

	//alloc 512B src memory and dst memory	
	src = dma_alloc_coherent(NULL, SIZE, &dma_src, GFP_KERNEL);  
	printk(KERN_INFO "src = 0x%x, dma_src = 0x%x\n",src, dma_src);	
	  
	dst = dma_alloc_coherent(NULL, SIZE, &dma_dst, GFP_KERNEL);  
	printk(KERN_INFO "dst = 0x%x, dma_dst = 0x%x\n",dst, dma_dst);	

	dma_cap_mask_t mask;  

	dma_cap_zero(mask);  
	dma_cap_set(DMA_MEMCPY, mask);//direction:memory to memory	
	chan = dma_request_channel(mask,NULL,NULL); //request a dma channel  
	printk(KERN_INFO "dma channel id = %d\n",chan->chan_id);  

	//flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;	
	flags = DMA_PREP_CONTINUE;
	dev = chan->device;  

	return 0;

out_free:
	simple_cleanup_module();
	return 0;
}

module_init(simple_init_module);
module_exit(simple_cleanup_module);

MODULE_LICENSE("GPL");


