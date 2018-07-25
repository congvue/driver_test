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
	{ 0x122184c2, "free_netdev" },
	{ 0x5c540aa3, "unregister_netdev" },
	{ 0xe240ccf1, "register_netdev" },
	{ 0x629aa42c, "alloc_netdev_mqs" },
	{ 0x7eae45e2, "ether_setup" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x27e1a049, "printk" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x5996abc2, "netif_rx" },
	{ 0xf578d967, "eth_type_trans" },
	{ 0x3ef796bc, "skb_put" },
	{ 0x7a52f21b, "dev_alloc_skb" },
	{ 0x9d669763, "memcpy" },
	{ 0x70f8e4f4, "__netif_schedule" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0x676bbc0f, "_set_bit" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

