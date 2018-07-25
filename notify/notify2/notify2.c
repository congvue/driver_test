#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

/*
* ע��֪ͨ��
*/
extern int register_test_notifier(struct notifier_block*);
extern int unregister_test_notifier(struct notifier_block*);

static int test_event1(struct notifier_block *this, unsigned long event, void *ptr)
{
  printk("In Event 1: Event Number is %d\n", event);
  return 0; 
}

static int test_event2(struct notifier_block *this, unsigned long event, void *ptr)
{
  printk("In Event 2: Event Number is %d\n", event);
  return 0; 
}

static int test_event3(struct notifier_block *this, unsigned long event, void *ptr)
{
  printk("In Event 3: Event Number is %d\n", event);
  return 0; 
}

/*
* �¼�1���ýڵ�ִ�еĺ���Ϊtest_event1
*/
static struct notifier_block test_notifier1 =
{
    .notifier_call = test_event1,
};

/*
* �¼�2���ýڵ�ִ�еĺ���Ϊtest_event1
*/
static struct notifier_block test_notifier2 =
{
    .notifier_call = test_event2,
};

/*
* �¼�3���ýڵ�ִ�еĺ���Ϊtest_event1
*/
static struct notifier_block test_notifier3 =
{
    .notifier_call = test_event3,
};

/*
* ����Щ�¼�����ע��
*/
static int __init reg_notifier(void)
{
  int err;
  printk("Begin to register:\n");
  
  err = register_test_notifier(&test_notifier1);
  if (err)
  {
    printk("register test_notifier1 error\n");
    return -1; 
  }
  printk("register test_notifier1 completed\n");

  err = register_test_notifier(&test_notifier2);
  if (err)
  {
    printk("register test_notifier2 error\n");
    return -1; 
  }
  printk("register test_notifier2 completed\n");

  err = register_test_notifier(&test_notifier3);
  if (err)
  {
    printk("register test_notifier3 error\n");
    return -1; 
  }
  printk("register test_notifier3 completed\n");
  
  return err;
}

/*
* ж�ظո�ע���˵�֪ͨ��
*/
static void __exit unreg_notifier(void)
{
  printk("Begin to unregister\n");
  unregister_test_notifier(&test_notifier1);
  unregister_test_notifier(&test_notifier2);
  unregister_test_notifier(&test_notifier3);
  printk("Unregister finished\n");
}

module_init(reg_notifier);
module_exit(unreg_notifier);