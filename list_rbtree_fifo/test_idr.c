#include<linux/idr.h>
#include "kn_common.h"

MODULE_LICENSE("Dual BSD/GPL");
struct student
{
    int id;
    char* name;
};

static int print_student(int, void*, void*);

static int testidr_init(void)
{
    DEFINE_IDR(idp);
    struct student *stu[4];
    //    struct student *stu_tmp;
    int id, ret, i;

    // init 4 struct student
    for (i=0; i<4; i++) {

        stu[i] = kmalloc(sizeof(struct student), GFP_KERNEL);
        stu[i]->id = i;
        stu[i]->name = "wyb";
    }

    // add 4 student to idr
    print_current_time(0);
    for (i=0; i < 4; i++) {

        do {
            if (!idr_pre_get(&idp, GFP_KERNEL))
                return -ENOSPC;
            ret = idr_get_new(&idp, stu[i], &id);
            printk(KERN_ALERT "id=%d\n", id);
        } while(ret == -EAGAIN);
    }

    // display all student in idr
    idr_for_each(&idp, print_student, NULL);

    idr_destroy(&idp);
    kfree(stu[0]);
    kfree(stu[1]);
    kfree(stu[2]);
    kfree(stu[3]);
    return 0;
}

static int print_student(int id, void *p, void *data)
{
    struct student* stu = p;
       
    printk(KERN_ALERT "=========================\n");
    print_current_time(0);
    printk(KERN_ALERT "id = %d\n", stu->id);
    printk(KERN_ALERT "name = %s\n", stu->name);
    printk(KERN_ALERT "=========================\n");

    return 0;
}

static void testidr_exit(void)
{
    printk(KERN_ALERT "*************************\n");
    print_current_time(0);
    printk(KERN_ALERT "testidr is exited!\n");
    printk(KERN_ALERT "*************************\n");
}

module_init(testidr_init);
module_exit(testidr_exit);