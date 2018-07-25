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
* �����Լ���֪ͨ��ͷ����Լ�ע���ж��֪ͨ�����������
*/

/*
* RAW_NOTIFIER_HEAD�Ƕ���һ��֪ͨ����ͷ����㣬
* ͨ�����ͷ���������ҵ�������е��������е�notifier_block
*/
static RAW_NOTIFIER_HEAD(test_chain);

/*
* �Զ����ע�ắ������notifier_block�ڵ�ӵ��ոն����test_chain�����������
* raw_notifier_chain_register�����notifier_chain_register
*/
int register_test_notifier(struct notifier_block *nb)
{
  return raw_notifier_chain_register(&test_chain, nb);
}
EXPORT_SYMBOL(register_test_notifier);

int unregister_test_notifier(struct notifier_block *nb)
{
  return raw_notifier_chain_unregister(&test_chain, nb);
}
EXPORT_SYMBOL(unregister_test_notifier);

/*
* �Զ����֪ͨ����ĺ�������֪ͨtest_chainָ��������е����нڵ�ִ����Ӧ�ĺ���
*/
int test_notifier_call_chain(unsigned long val, void *v)
{
  return raw_notifier_call_chain(&test_chain, val, v);
}
EXPORT_SYMBOL(test_notifier_call_chain);

/*
* init and exit 
*/
static int __init init_notifier(void)
{
  printk("init_notifier\n");
  return 0;
}

static void __exit exit_notifier(void)
{
    printk("exit_notifier\n");
}

module_init(init_notifier);
module_exit(exit_notifier);