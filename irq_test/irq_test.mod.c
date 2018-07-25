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
	{ 0x2dcf6fe6, "i2c_del_driver" },
	{ 0x94fbf8ee, "i2c_register_driver" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0x74c97f9c, "_raw_spin_unlock_irqrestore" },
	{ 0xfcec0987, "enable_irq" },
	{ 0xbd7083bc, "_raw_spin_lock_irqsave" },
	{ 0x332ceef9, "kmalloc_caches" },
	{ 0x6c525c85, "kmem_cache_alloc" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x33b27841, "input_unregister_device" },
	{ 0x4257f7e6, "dev_set_drvdata" },
	{ 0x27e1a049, "printk" },
	{ 0x8245763f, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

