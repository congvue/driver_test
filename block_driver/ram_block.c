
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/blkdev.h>
#include <linux/bio.h>

#define RAMDISK_MAJOR    0        /*���豸������0���ں˶�̬����һ�����豸��*/
#define RAMDISK_NAME    "my2440_ramdisk"    /*�豸����*/
#define RAMDISK_SIZE    (4 * 1024 * 1024)   /*������̵Ĵ�С,��4M*/

static int ramdisk_major = RAMDISK_MAJOR;   /*�������涯̬��������豸��*/
static struct class *ramdisk_class;         /*����һ���豸��,����/dev�¶�̬�����豸�ڵ�*/

static struct gendisk *my2440_ramdiak;      /*����һ��gendisk�ṹ��������ʾһ�������豸*/
static struct request_queue *ramdisk_queue; /*��������豸���������*/

unsigned char *disk_data;/*����һ��ָ������ʾramdisk���豸���ڴ��е���*/

/*���豸���������ṹ�壬��ʵ����Ҫ��ʲô���������������Ϊ��*/
static struct block_device_operations ramdisk_fops = 
{
    .owner    = THIS_MODULE,
};

static int __init ramdisk_init(void)
{
    int ret;

    /*���豸����ע��, ע��������豸������ע����2.6�ں����ǿ�ѡ�ģ�
    �ú������ں��ṩ������ʹ����Ϊ�˻��һ����̬���ɵ����豸��*/
    ramdisk_major = register_blkdev(RAMDISK_MAJOR, RAMDISK_NAME);
    if(ramdisk_major <= 0)
    {
        return ramdisk_major;
    }

    /*��̬����һ���豸�ڵ㣬���ַ����豸һ��*/
    ramdisk_class = class_create(THIS_MODULE, RAMDISK_NAME);
    if(IS_ERR(ramdisk_class))
    {
        ret = -1;
        goto err_class;
    }
    device_create(ramdisk_class, NULL, MKDEV(ramdisk_major, 0), NULL, RAMDISK_NAME);

    /*RamDisk������������ʵ��豸����˲�ʹ��������еĴ���ʽ����ʹ����������ķ�ʽ*/
    ramdisk_queue = blk_alloc_queue(GFP_KERNEL);/*����ramdisk_queue*/

    if(!ramdisk_queue)
    {
        ret = -ENOMEM;
        goto err_queue;
    }
    blk_queue_make_request(ramdisk_queue, &ramdisk_make_request);/*���������캯��*/

    /*����gendisk���ú������ں��ṩ������Ϊ�����豸�Ĵ��豸������(���ߴ��̵ķ�������)
    ע��һ�������ʹ���һ�����豸������ָ���������Ժ�Ͳ��ܱ��޸���*/
    my2440_ramdiak = alloc_disk(1);

    if(!my2440_ramdiak)
    {
        ret = -ENOMEM;
        goto err_alloc;
    }

    /*��ʼ��gendisk*/
    my2440_ramdiak->major = ramdisk_major;    /*����ָ�������豸�ž��������涯̬��ȡ�����豸��*/
    my2440_ramdiak->first_minor    = 0;       /*ָ����һ�����豸��Ϊ0*/
    my2440_ramdiak->fops = &ramdisk_fops;     /*ָ�����豸�����Եײ�Ӳ�������Ľṹ��ָ�룬�����ں�������*/
    my2440_ramdiak->queue = ramdisk_queue;    /*ָ����ʼ���õ��������*/
    sprintf(my2440_ramdiak->disk_name, RAMDISK_NAME);/*ָ�������豸������*/

    /*���ô����豸��������С,�ú������ں��ṩ��
    ע��ú�������512�ֽ�Ϊ1��������λ���д���ģ���Ϊ�ں�Ҫ�����*/
    set_capacity(my2440_ramdiak, RAMDISK_SIZE >> 9);/*����9λ���ǳ���512*/

    /*���gendisk��ϵͳ��, �ú������ں��ṩ*/
    add_disk(my2440_ramdiak);

    return 0;

/*������*/
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
    /*ɾ�������豸*/
    del_gendisk(my2440_ramdiak);
    put_disk(my2440_ramdiak);

    /*����������*/
    blk_cleanup_queue(ramdisk_queue);

    /*����豸��*/
    device_destroy(ramdisk_class, MKDEV(ramdisk_major, 0));
    class_destroy(ramdisk_class);

    /*ע�����豸*/
    unregister_blkdev(ramdisk_major, RAMDISK_NAME);
}

module_init(ramdisk_init);
module_exit(ramdisk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huang Gang");
MODULE_DESCRIPTION("My2440 RamDisk Driver");