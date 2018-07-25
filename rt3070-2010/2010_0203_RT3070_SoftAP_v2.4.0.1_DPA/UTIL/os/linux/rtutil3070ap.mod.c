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
	{ 0x2ee08479, "module_layout" },
	{ 0xc5008d43, "register_netdevice" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x97255bdf, "strlen" },
	{ 0x9c0ba064, "malloc_sizes" },
	{ 0xcf0db3a7, "dev_get_by_name" },
	{ 0x915599b1, "skb_clone" },
	{ 0xfc00c9cd, "usb_kill_urb" },
	{ 0x30e44759, "filp_close" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x86cb7b28, "init_timer_key" },
	{ 0x999e8297, "vfree" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xccee0a11, "kthread_create_on_node" },
	{ 0x7d11c268, "jiffies" },
	{ 0xb445bee8, "netif_rx" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xf7b574c2, "del_timer_sync" },
	{ 0x86c2ac43, "dev_alloc_skb" },
	{ 0xf4cc6142, "usb_deregister" },
	{ 0x27e1a049, "printk" },
	{ 0x65fbd4d6, "kthread_stop" },
	{ 0x7febe6ce, "free_netdev" },
	{ 0x328a05f1, "strncpy" },
	{ 0xe96533ad, "register_netdev" },
	{ 0xb3a2222b, "wireless_send_event" },
	{ 0x9959d18d, "usb_control_msg" },
	{ 0x1ec7c085, "skb_push" },
	{ 0xda882b40, "dev_close" },
	{ 0x6ed4557e, "mod_timer" },
	{ 0x22d88e1d, "add_timer" },
	{ 0xb6d03316, "usb_free_coherent" },
	{ 0x3e5fae22, "init_net" },
	{ 0x5597bd60, "dev_kfree_skb_any" },
	{ 0x61651be, "strcat" },
	{ 0xd429af30, "skb_copy_expand" },
	{ 0xf49bf07b, "usb_submit_urb" },
	{ 0x4bdf097d, "__alloc_skb" },
	{ 0x8858a5e5, "usb_get_dev" },
	{ 0x19211211, "usb_put_dev" },
	{ 0x60243ef7, "eth_type_trans" },
	{ 0x65d84862, "wake_up_process" },
	{ 0x94dea09a, "kmem_cache_alloc_trace" },
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xd74f18a8, "usb_register_driver" },
	{ 0x51793626, "unregister_netdev" },
	{ 0x99bb8806, "memmove" },
	{ 0x5a5c1367, "usb_alloc_coherent" },
	{ 0xb3e4f967, "skb_put" },
	{ 0x3470f355, "usb_free_urb" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xf4faf2ea, "usb_alloc_urb" },
	{ 0xe914e41e, "strcpy" },
	{ 0x574e0fcf, "filp_open" },
	{ 0xe1dc38ab, "alloc_etherdev_mqs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

