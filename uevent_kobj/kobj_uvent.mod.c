#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb76c596d, "module_layout" },
	{ 0x63ec13e7, "sysfs_create_group" },
	{ 0xb7951ef2, "device_create_file" },
	{ 0x4c57bdeb, "device_create" },
	{ 0x4efb57e7, "__class_register" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x27e1a049, "printk" },
	{ 0x3355e210, "kobject_uevent_env" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

