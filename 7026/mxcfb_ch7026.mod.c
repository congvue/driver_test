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
	{ 0x8cc7be14, "registered_fb" },
	{ 0x6c61ce70, "num_registered_fb" },
	{ 0xf4ee3c50, "fb_show_logo" },
	{ 0x5934392b, "fb_register_client" },
	{ 0xc68e6b93, "i2c_smbus_write_byte_data" },
	{ 0xc03be003, "i2c_smbus_read_byte_data" },
	{ 0xf9a482f9, "msleep" },
	{ 0x27e1a049, "printk" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xc631580a, "console_unlock" },
	{ 0xdb2ea21b, "fb_blank" },
	{ 0xd411df38, "fb_set_var" },
	{ 0xfbaaf01e, "console_lock" },
	{ 0xc52f5714, "fb_videomode_to_var" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x6f142365, "regulator_put" },
	{ 0xcc36f32e, "fb_unregister_client" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:ch7026");
