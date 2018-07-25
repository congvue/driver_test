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
	{ 0x2ee08479, "module_layout" },
	{ 0xbe8524ca, "sock_release" },
	{ 0xa818f457, "init_net" },
	{ 0xd0018717, "netlink_kernel_create" },
	{ 0x6583f0d0, "kfree_skb" },
	{ 0x48d316aa, "wait_for_completion_timeout" },
	{ 0x41e92619, "__init_waitqueue_head" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x22b5dff0, "netlink_unicast" },
	{ 0x27e1a049, "printk" },
	{ 0x9d669763, "memcpy" },
	{ 0xdbf7ab85, "skb_put" },
	{ 0x35739e0d, "__alloc_skb" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

