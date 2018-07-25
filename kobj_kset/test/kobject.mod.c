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
	{ 0x2f2270e8, "module_layout" },
	{ 0x202c73d0, "kobject_put" },
	{ 0xe84dd843, "kobject_del" },
	{ 0x332ceef9, "kmalloc_caches" },
	{ 0x7eed8d81, "kobject_init_and_add" },
	{ 0x7d2e9a16, "kset_unregister" },
	{ 0x6c525c85, "kmem_cache_alloc" },
	{ 0xe8d65eb1, "kset_create_and_add" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x42224298, "sscanf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x37a0cba, "kfree" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

