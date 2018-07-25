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
#include "demo.h"


struct simple_dev *simple_device;
static unsigned char simple_inc = 0;
static char *buffer = NULL;
static char *buffer_area = NULL;

int simple_open(struct inode *inode, struct file *filp)
{
	struct simple_dev *dev;
	simple_inc++;

	dev = container_of(inode->i_cdev, struct simple_dev, cdev);

	filp->private_data = dev;

	return 0;

}

int simple_release(struct inode *inode, struct file *filp)
{
	simple_inc--;

	return 0;
	
}

static int simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void *)((unsigned long)buffer_area)) >> PAGE_SHIFT,
		vma->vm_end - vma->vm_start, PAGE_SHARED);
/*
virt_to_phys((void *)((unsigned long)buffer_area)) 是将一段虚拟内存转换为物理内存
然后通过>> PAGE_SHIFT获得页帧号
remap_pfn_range和io_remap_pfn_range负责为一段物理地址建立新的页表


*/

	if(ret != 0) {
		return -EAGAIN;
	}

	return 0;
	
}

struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.open = simple_open, 
	.mmap = simple_mmap,
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

	for(virt_addr = (unsigned long)buffer_area; virt_addr < (unsigned long)buffer_area + 4096; virt_addr += PAGE_SIZE) {
		SetPageReserved(virt_to_page(virt_addr));
	}

	if(buffer) kfree(buffer);

	unregister_chrdev_region(devno, 1);
	

}

int simple_init_module(void)
{
	int result;
	dev_t dev = 0;

	int i;
	unsigned long virt_addr;

	dev = MKDEV(simple_MAJOR, simple_MINOR);
	result = register_chrdev_region(dev, 1, "mmap");
	if(result < 0) {
		printk("register chrdev fail\n");
		goto out_free;
	}

	printk("major = %d minor = %d\n",MAJOR(dev), MINOR(dev));

	simple_device = kmalloc(sizeof(struct simple_dev), GFP_KERNEL);
	if(!simple_device) {
		printk("kmalloc mem to simple_device fail\n");
		goto out_free;
	}

	memset(simple_device, 0, sizeof(struct simple_dev));

	sema_init(&simple_device->sem, 1);

	cdev_init(&simple_device->cdev, &simple_fops);

	simple_device->cdev.owner =  THIS_MODULE;
	simple_device->cdev.ops = &simple_fops;
	result = cdev_add(&simple_device->cdev, dev, 1);
	if(result) {
		printk("cdev add fail\n");
		goto out_free;
	}

	buffer = kmalloc(4096, GFP_KERNEL);
	printk("mmap buffer = 0x%p\n",buffer);

	buffer_area = (int *)(((unsigned long)buffer + PAGE_SIZE - 1) & PAGE_MASK);

	for(virt_addr = (unsigned long)buffer_area; virt_addr < (unsigned long)buffer_area + 4096; virt_addr += PAGE_SIZE) {
		SetPageReserved(virt_to_page(virt_addr));//将页面设置为保留，防止被映射到用户空间的页面被swap out 出去
	}

	memset(buffer, 'C', 100);

	return 0;

out_free:
	simple_cleanup_module();
	return result;
}

module_init(simple_init_module);
module_exit(simple_cleanup_module);

MODULE_LICENSE("GPL");


