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
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0xf1c86f14, "v4l2_fh_open" },
	{ 0xf62e7e5a, "video_ioctl2" },
	{ 0xfd0245cf, "video_unregister_device" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x37a0cba, "kfree" },
	{ 0xb8890c3d, "v4l2_device_unregister" },
	{ 0x79c6bafc, "v4l2_ctrl_handler_free" },
	{ 0xcc660c5, "video_device_release" },
	{ 0x4257f7e6, "dev_set_drvdata" },
	{ 0x640cf376, "__video_register_device" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xe601ceec, "video_device_alloc" },
	{ 0x41e92619, "__init_waitqueue_head" },
	{ 0xeac5046c, "__mutex_init" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x47e393ba, "v4l2_ctrl_new_custom" },
	{ 0xb2624811, "v4l2_ctrl_new_std" },
	{ 0xf73a0ef9, "v4l2_ctrl_handler_init" },
	{ 0xa24032bd, "v4l2_device_register" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x9c55cec, "schedule_timeout_interruptible" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x3fdacc6f, "add_wait_queue" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0x35c2ba9e, "refrigerator" },
	{ 0x718565d5, "remove_wait_queue" },
	{ 0x1d2e87c6, "do_gettimeofday" },
	{ 0x701d0ebd, "snprintf" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0xff178f6, "__aeabi_idivmod" },
	{ 0x97255bdf, "strlen" },
	{ 0xc27487dd, "__bug" },
	{ 0xb7ddbd6, "mutex_unlock" },
	{ 0x7f14cbec, "mutex_lock" },
	{ 0x7d11c268, "jiffies" },
	{ 0x72542c85, "__wake_up" },
	{ 0xcf569186, "wake_up_process" },
	{ 0xef799a00, "kthread_create_on_node" },
	{ 0x9c5dfbd2, "kthread_stop" },
	{ 0x74c97f9c, "_raw_spin_unlock_irqrestore" },
	{ 0xbd7083bc, "_raw_spin_lock_irqsave" },
	{ 0x9028f079, "v4l2_fh_release" },
	{ 0xf3ce75, "v4l2_fh_is_singular" },
	{ 0x9d669763, "memcpy" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0xf5ef842e, "v4l_bound_align_image" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x8245763f, "dev_get_drvdata" },
	{ 0x90ddfad0, "video_devdata" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x27e1a049, "printk" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

