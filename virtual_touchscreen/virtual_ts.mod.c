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
	{ 0x33b27841, "input_unregister_device" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x86626159, "class_destroy" },
	{ 0xbb3ad7bb, "device_destroy" },
	{ 0x4db4a8f, "cdev_del" },
	{ 0x4c57bdeb, "device_create" },
	{ 0x56303c2f, "__class_create" },
	{ 0xc2a7bd1d, "cdev_add" },
	{ 0x2a759baa, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xd1145227, "input_register_device" },
	{ 0xda4006c6, "input_set_abs_params" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x75b92e70, "input_allocate_device" },
	{ 0xfd40dd03, "input_event" },
	{ 0x42224298, "sscanf" },
	{ 0x27e1a049, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F796726AB6AF3326360F2E2");
