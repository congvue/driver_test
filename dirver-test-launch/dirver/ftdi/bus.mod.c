#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe3b0c00b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xbdd94baa, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0x5fc56a46, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9c0bd51f, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xc09b4ac6, __VMLINUX_SYMBOL_STR(driver_register) },
	{ 0x694be42a, __VMLINUX_SYMBOL_STR(usb_show_dynids) },
	{ 0x5651c1f, __VMLINUX_SYMBOL_STR(usb_store_new_id) },
	{ 0xf2d995ad, __VMLINUX_SYMBOL_STR(tty_register_device) },
	{ 0x8bcc4547, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xb4443d0b, __VMLINUX_SYMBOL_STR(usb_autopm_put_interface) },
	{ 0x830431ea, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x4877e51c, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0x300a151d, __VMLINUX_SYMBOL_STR(tty_unregister_device) },
	{ 0x2868288c, __VMLINUX_SYMBOL_STR(usb_autopm_get_interface) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4F0D18FF0F8B337D91F2A91");
