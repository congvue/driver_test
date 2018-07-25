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
	{ 0xeb1326f8, "kthread_stop" },
	{ 0xfde1ca8d, "wake_up_process" },
	{ 0xe33ad6ce, "kthread_create_on_node" },
	{ 0x41e92619, "__init_waitqueue_head" },
	{ 0x2fd61cd, "wait_for_completion" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x4f7f2d1b, "complete" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0xcc5005fe, "msleep_interruptible" },
	{ 0x27e1a049, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

