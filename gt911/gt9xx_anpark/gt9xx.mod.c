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
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x2dcf6fe6, "i2c_del_driver" },
	{ 0x94fbf8ee, "i2c_register_driver" },
	{ 0x7426f76c, "init_timer_key" },
	{ 0x173f2452, "__alloc_workqueue_key" },
	{ 0x332ceef9, "kmalloc_caches" },
	{ 0xa06d229d, "hrtimer_init" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0xd1145227, "input_register_device" },
	{ 0xda4006c6, "input_set_abs_params" },
	{ 0x75b92e70, "input_allocate_device" },
	{ 0xc5ff4c73, "proc_create_data" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x6c525c85, "kmem_cache_alloc" },
	{ 0xe1d61c3a, "cancel_delayed_work_sync" },
	{ 0x311b7963, "_raw_spin_unlock" },
	{ 0xc2161e33, "_raw_spin_lock" },
	{ 0x1fd90dcc, "queue_delayed_work" },
	{ 0x71c90087, "memcmp" },
	{ 0xfd40dd03, "input_event" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x5f754e5a, "memset" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x9d669763, "memcpy" },
	{ 0x8d1728d4, "i2c_transfer" },
	{ 0xf9a482f9, "msleep" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x74c97f9c, "_raw_spin_unlock_irqrestore" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xbd7083bc, "_raw_spin_lock_irqsave" },
	{ 0xc67ef753, "hrtimer_start" },
	{ 0x1375b454, "queue_work" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xfe990052, "gpio_free" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x37a0cba, "kfree" },
	{ 0x33b27841, "input_unregister_device" },
	{ 0x4257f7e6, "dev_set_drvdata" },
	{ 0x27e1a049, "printk" },
	{ 0x9200c5ce, "hrtimer_cancel" },
	{ 0x18792500, "destroy_workqueue" },
	{ 0xcc36f32e, "fb_unregister_client" },
	{ 0x8245763f, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

