#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0xc3043f12, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x95799440, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x42bf6cfc, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x463c58ef, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x8f1fa55b, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x21d6c5e4, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x97e1157e, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x90412057, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfb961d14, __VMLINUX_SYMBOL_STR(__arm_ioremap) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EE806AE6F51F8EC30610843");
