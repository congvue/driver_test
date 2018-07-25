
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/blkdev.h>
#include <linux/bio.h>

#define RAMDISK_MAJOR    0        /*主设备号设置0让内核动态产生一个主设备号*/
#define RAMDISK_NAME    "my2440_ramdisk"    /*设备名称*/
#define RAMDISK_SIZE    (4 * 1024 * 1024)   /*虚拟磁盘的大小,共4M*/

static int ramdisk_major = RAMDISK_MAJOR;   /*用来保存动态分配的主设备号*/
static struct class *ramdisk_class;         /*定义一个设备类,好在/dev下动态生成设备节点*/

static struct gendisk *my2440_ramdiak;      /*定义一个gendisk结构体用来表示一个磁盘设备*/
static struct request_queue *ramdisk_queue; /*定义磁盘设备的请求队列*/

unsigned char *disk_data;/*定义一个指针来表示ramdisk块设备在内存中的域*/

/*块设备驱动操作结构体，其实不需要做什么操作，这里就设置为空*/
static struct block_device_operations ramdisk_fops = 
{
    .owner    = THIS_MODULE,
};

static int __init ramdisk_init(void)
{
    int ret;

    /*块设备驱动注册, 注意这个块设备驱动的注册在2.6内核中是可选的，
    该函数由内核提供。这里使用是为了获得一个动态生成的主设备号*/
    ramdisk_major = register_blkdev(RAMDISK_MAJOR, RAMDISK_NAME);
    if(ramdisk_major <= 0)
    {
        return ramdisk_major;
    }

    /*动态创建一个设备节点，跟字符型设备一样*/
    ramdisk_class = class_create(THIS_MODULE, RAMDISK_NAME);
    if(IS_ERR(ramdisk_class))
    {
        ret = -1;
        goto err_class;
    }
    device_create(ramdisk_class, NULL, MKDEV(ramdisk_major, 0), NULL, RAMDISK_NAME);

    /*RamDisk属真正随机访问的设备，因此不使用请求队列的处理方式，而使用制造请求的方式*/
    ramdisk_queue = blk_alloc_queue(GFP_KERNEL);/*分配ramdisk_queue*/

    if(!ramdisk_queue)
    {
        ret = -ENOMEM;
        goto err_queue;
    }
    blk_queue_make_request(ramdisk_queue, &ramdisk_make_request);/*绑定请求制造函数*/

    /*分配gendisk，该函数由内核提供，参数为磁盘设备的次设备号数量(或者磁盘的分区数量)
    注意一个分区就代表一个次设备，这里指定数量后以后就不能被修改了*/
    my2440_ramdiak = alloc_disk(1);

    if(!my2440_ramdiak)
    {
        ret = -ENOMEM;
        goto err_alloc;
    }

    /*初始化gendisk*/
    my2440_ramdiak->major = ramdisk_major;    /*这里指定的主设备号就是在上面动态获取的主设备号*/
    my2440_ramdiak->first_minor    = 0;       /*指定第一个次设备号为0*/
    my2440_ramdiak->fops = &ramdisk_fops;     /*指定块设备驱动对底层硬件操作的结构体指针，定义在后面来讲*/
    my2440_ramdiak->queue = ramdisk_queue;    /*指定初始化好的请求队列*/
    sprintf(my2440_ramdiak->disk_name, RAMDISK_NAME);/*指定磁盘设备的名称*/

    /*设置磁盘设备的容量大小,该函数由内核提供。
    注意该函数是以512字节为1个扇区单位进行处理的，因为内核要求如此*/
    set_capacity(my2440_ramdiak, RAMDISK_SIZE >> 9);/*右移9位就是除以512*/

    /*添加gendisk到系统中, 该函数由内核提供*/
    add_disk(my2440_ramdiak);

    return 0;

/*错误处理*/
err_class:
    unregister_blkdev(ramdisk_major, RAMDISK_NAME);
err_queue:
    device_destroy(ramdisk_class, MKDEV(ramdisk_major, 0));
    class_destroy(ramdisk_class);
err_alloc:
    blk_cleanup_queue(ramdisk_queue);

    return ret;
}

static void __exit ramdisk_exit(void)
{    
    /*删除磁盘设备*/
    del_gendisk(my2440_ramdiak);
    put_disk(my2440_ramdiak);

    /*清除请求队列*/
    blk_cleanup_queue(ramdisk_queue);

    /*清除设备类*/
    device_destroy(ramdisk_class, MKDEV(ramdisk_major, 0));
    class_destroy(ramdisk_class);

    /*注销块设备*/
    unregister_blkdev(ramdisk_major, RAMDISK_NAME);
}

module_init(ramdisk_init);
module_exit(ramdisk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huang Gang");
MODULE_DESCRIPTION("My2440 RamDisk Driver");