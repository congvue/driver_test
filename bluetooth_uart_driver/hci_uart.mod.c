#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb76c596d, "module_layout" },
	{ 0x2a92bb89, "skb_queue_head" },
	{ 0xb48a77ef, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x311b7963, "_raw_spin_unlock" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x1b945caf, "del_timer" },
	{ 0x520d2850, "__alloc_workqueue_key" },
	{ 0x8e06393d, "hci_register_dev" },
	{ 0xe1e80faf, "sock_release" },
	{ 0x5131b3e3, "tty_unthrottle" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0xb78c61e8, "param_ops_bool" },
	{ 0x7426f76c, "init_timer_key" },
	{ 0xe1d61c3a, "cancel_delayed_work_sync" },
	{ 0x473cc338, "hci_recv_frame" },
	{ 0xb6e256ce, "hci_unregister_dev" },
	{ 0xa120d33c, "tty_unregister_ldisc" },
	{ 0x7d11c268, "jiffies" },
	{ 0xa0c4d14c, "skb_trim" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xa4476cec, "del_timer_sync" },
	{ 0x9fad2d38, "skb_queue_purge" },
	{ 0x74c97f9c, "_raw_spin_unlock_irqrestore" },
	{ 0x27e1a049, "printk" },
	{ 0x71c90087, "memcmp" },
	{ 0xa8984dfa, "tty_ldisc_flush" },
	{ 0xab8fd048, "sock_sendmsg" },
	{ 0x5069b1a0, "n_tty_ioctl_helper" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0xe2e1054f, "hci_alloc_dev" },
	{ 0xd4847e16, "skb_push" },
	{ 0xa13e4fcb, "destroy_workqueue" },
	{ 0x53985936, "mod_timer" },
	{ 0x8a7c6f1, "add_timer" },
	{ 0x5f68ac8, "skb_pull" },
	{ 0xb272abe1, "flush_workqueue" },
	{ 0x295f6e04, "skb_queue_tail" },
	{ 0xbcf83ac5, "kmem_cache_alloc" },
	{ 0x31c2aa7f, "__alloc_skb" },
	{ 0x42f136a3, "tty_driver_flush_buffer" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x2f414d5, "kfree_skb" },
	{ 0xc2161e33, "_raw_spin_lock" },
	{ 0xbd7083bc, "_raw_spin_lock_irqsave" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x215ebd78, "bitrev16" },
	{ 0x9e731d90, "sock_create" },
	{ 0x62a0d669, "skb_dequeue" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xca54fee, "_test_and_set_bit" },
	{ 0x461b045a, "hci_free_dev" },
	{ 0x9c28a154, "tty_register_ldisc" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0x5a7293c0, "skb_put" },
	{ 0xf279bb7e, "queue_delayed_work" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "436E1B1F9FA3FDC8F628F75");
