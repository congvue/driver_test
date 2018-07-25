#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/i2c.h>
#include <linux/input.h>


struct device *ts_dev = NULL;

char * s_c[2];

static ssize_t send(struct device *dev,struct device_attribute *attr,const char *buf, size_t count)
{
    s_c[0] = "song_hello";
    s_c[1] = NULL;
    
    kobject_uevent_env(&dev->kobj, KOBJ_CHANGE, s_c);

    printk("buf = %s\n",buf);
    
    return count;
}

static DEVICE_ATTR(song, S_IRUGO|S_IWUSR, NULL, send);


static const struct attribute *song_event_attr[] = {
        &dev_attr_song.attr,
        NULL,
};


static const struct attribute_group song_event_attr_group = {

        .attrs = (struct attribute **) song_event_attr,
};


static struct class song_event_class = {
        .name =         "song_event",
        .owner =        THIS_MODULE,
};


static int __init song_uevent_init(void)
{
    int ret = 0;
    
    ret = class_register(&song_event_class);

    if (ret < 0) {
        printk(KERN_ERR "song_event: class_register fail\n");

        return ret;
    }


    ts_dev = device_create(&song_event_class, NULL, MKDEV(240, 0), NULL, "song_event");


    //device_create_file(ts_dev,&dev_attr_song);

    if (ts_dev) {
        ret = sysfs_create_group(&ts_dev->kobj, &song_event_attr_group);
        if(ret < 0) {
            printk(KERN_ERR "song_event:sysfs_create_group fail\r\n");
            return ret;
        }
    } else {
        printk(KERN_ERR "song_event:device_create fail\r\n");

        ret = -1;

        return ret;
    }


    return 0;
}


module_init(song_uevent_init);
MODULE_AUTHOR("<wmzjzwlzs@163.com>");
MODULE_DESCRIPTION("song self event driver");
MODULE_LICENSE("GPL");

