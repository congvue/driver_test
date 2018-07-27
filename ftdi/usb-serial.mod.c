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
	{ 0xe3b0c00b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe995ca22, __VMLINUX_SYMBOL_STR(tty_port_tty_get) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x97ad4574, __VMLINUX_SYMBOL_STR(bus_register) },
	{ 0x2b4aa71c, __VMLINUX_SYMBOL_STR(usb_serial_generic_write) },
	{ 0xdb428d43, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x5fc56a46, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x710dfb3f, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xc068440e, __VMLINUX_SYMBOL_STR(__kfifo_alloc) },
	{ 0x9ac4dc4, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x4dda1cf6, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x431ae3a9, __VMLINUX_SYMBOL_STR(seq_puts) },
	{ 0x6e4eaad, __VMLINUX_SYMBOL_STR(tty_port_open) },
	{ 0x7d2c0224, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x719b552e, __VMLINUX_SYMBOL_STR(tty_port_hangup) },
	{ 0x4205ad24, __VMLINUX_SYMBOL_STR(cancel_work_sync) },
	{ 0xe72f62e, __VMLINUX_SYMBOL_STR(tty_register_driver) },
	{ 0xe2605162, __VMLINUX_SYMBOL_STR(usb_serial_generic_process_read_urb) },
	{ 0xa5652d15, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x2868288c, __VMLINUX_SYMBOL_STR(usb_autopm_get_interface) },
	{ 0xf701149f, __VMLINUX_SYMBOL_STR(put_tty_driver) },
	{ 0x37e50e9, __VMLINUX_SYMBOL_STR(usb_serial_generic_chars_in_buffer) },
	{ 0x41f3df3b, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x5ccc3fa1, __VMLINUX_SYMBOL_STR(tty_set_operations) },
	{ 0x8786a2ee, __VMLINUX_SYMBOL_STR(tty_port_close) },
	{ 0x575a4df5, __VMLINUX_SYMBOL_STR(usb_serial_generic_resume) },
	{ 0xf96179b, __VMLINUX_SYMBOL_STR(device_del) },
	{ 0x183755e1, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xeb3a8ee6, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x948fccad, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xcda65b3c, __VMLINUX_SYMBOL_STR(usb_unpoison_urb) },
	{ 0xf804b5da, __VMLINUX_SYMBOL_STR(usb_poison_urb) },
	{ 0x70e0ec02, __VMLINUX_SYMBOL_STR(seq_putc) },
	{ 0xa44a2de9, __VMLINUX_SYMBOL_STR(tty_port_init) },
	{ 0x6ec3c160, __VMLINUX_SYMBOL_STR(tty_port_destroy) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0xdea829bd, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x570717b8, __VMLINUX_SYMBOL_STR(tty_vhangup) },
	{ 0x6c33396b, __VMLINUX_SYMBOL_STR(device_add) },
	{ 0xe6ae3bb, __VMLINUX_SYMBOL_STR(bus_unregister) },
	{ 0xf735d5d1, __VMLINUX_SYMBOL_STR(usb_serial_generic_close) },
	{ 0xe547025d, __VMLINUX_SYMBOL_STR(usb_match_id) },
	{ 0xc307a1e4, __VMLINUX_SYMBOL_STR(usb_serial_generic_open) },
	{ 0xfeb75ab3, __VMLINUX_SYMBOL_STR(tty_port_tty_wakeup) },
	{ 0x2f9381e9, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x830431ea, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x9828a530, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x67b27ec1, __VMLINUX_SYMBOL_STR(tty_std_termios) },
	{ 0x6e909c7e, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x4e5f33d8, __VMLINUX_SYMBOL_STR(put_device) },
	{ 0x1e659d14, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x23f13d83, __VMLINUX_SYMBOL_STR(usb_serial_generic_write_bulk_callback) },
	{ 0x8c68aba5, __VMLINUX_SYMBOL_STR(tty_port_install) },
	{ 0x9c0bd51f, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xb41f6dd7, __VMLINUX_SYMBOL_STR(usb_get_intf) },
	{ 0x30e74134, __VMLINUX_SYMBOL_STR(tty_termios_copy_hw) },
	{ 0xdb760f52, __VMLINUX_SYMBOL_STR(__kfifo_free) },
	{ 0x9ee7d4ac, __VMLINUX_SYMBOL_STR(usb_serial_generic_read_bulk_callback) },
	{ 0x8424e753, __VMLINUX_SYMBOL_STR(tty_unregister_driver) },
	{ 0xd85cd67e, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x5f27cea8, __VMLINUX_SYMBOL_STR(__tty_alloc_driver) },
	{ 0x19a304ba, __VMLINUX_SYMBOL_STR(usb_disabled) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0xeaf9bce, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x560345ca, __VMLINUX_SYMBOL_STR(device_initialize) },
	{ 0x99ff21ce, __VMLINUX_SYMBOL_STR(usb_match_one_id) },
	{ 0xb70f91ef, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x75f90df2, __VMLINUX_SYMBOL_STR(tty_kref_put) },
	{ 0xa93a685d, __VMLINUX_SYMBOL_STR(driver_attach) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x676bbc0f, __VMLINUX_SYMBOL_STR(_set_bit) },
	{ 0x98d95e8d, __VMLINUX_SYMBOL_STR(usb_serial_generic_wait_until_sent) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x759fe9c8, __VMLINUX_SYMBOL_STR(dev_set_name) },
	{ 0x4c4beb4d, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xfe71047a, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x99e416a4, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0xb4443d0b, __VMLINUX_SYMBOL_STR(usb_autopm_put_interface) },
	{ 0x34a8380a, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x1866e34, __VMLINUX_SYMBOL_STR(usb_put_intf) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "684BADB7D67C9F939FEBD65");
