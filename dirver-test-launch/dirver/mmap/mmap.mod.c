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
	{ 0xb48a77ef, "kmalloc_caches" },
	{ 0x5f754e5a, "memset" },
	{ 0x32e78a59, "cdev_add" },
	{ 0x88b2953c, "cdev_init" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xbcf83ac5, "kmem_cache_alloc" },
	{ 0x27e1a049, "printk" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x959c383d, "mem_map" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x37a0cba, "kfree" },
	{ 0xb3f8ec84, "cdev_del" },
	{ 0x765f75fb, "pgprot_user" },
	{ 0x20bae612, "remap_pfn_range" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

