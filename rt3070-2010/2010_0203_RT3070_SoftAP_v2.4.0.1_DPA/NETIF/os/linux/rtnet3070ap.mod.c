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
	{ 0x2ee08479, "module_layout" },
	{ 0x52d9a0c1, "NICInitializeAdapter" },
	{ 0xf9a482f9, "msleep" },
	{ 0x6c4d1b13, "RTMPSetPhyMode" },
	{ 0x1f0695db, "RTUSBBulkReceive" },
	{ 0x698443e5, "AsicSwitchChannel" },
	{ 0x2fe9a0eb, "NICInitAsicFromEEPROM" },
	{ 0xeebc375, "CMDHandler" },
	{ 0xb1af2d9, "RTMPAPIoctlRF" },
	{ 0xa88b352e, "RTMPEnableRxTx" },
	{ 0xce428d58, "RtmpOSFileClose" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x866119ef, "RtmpOSFileRead" },
	{ 0xf61765ed, "dev_set_drvdata" },
	{ 0x79a1cf01, "ba_reordering_resource_init" },
	{ 0xb4736c29, "RTMPAPQueryInformation" },
	{ 0x55fcfffd, "RTMPIoctlGetMacTable" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x38b4b635, "RtmpOSTaskKill" },
	{ 0x7879c0f5, "ba_reordering_resource_release" },
	{ 0xd9317301, "APMakeAllBssBeacon" },
	{ 0x9c0ba064, "malloc_sizes" },
	{ 0x718565d5, "remove_wait_queue" },
	{ 0xa1df5836, "RTMPusecDelay" },
	{ 0x99709483, "N_ChannelCheck" },
	{ 0x6b18d14b, "RTMP_11N_D3_TimerInit" },
	{ 0x2e96c910, "netif_carrier_on" },
	{ 0x8bd94317, "_raw_spin_lock_bh" },
	{ 0x910ac94c, "RtmpOSNetDevFree" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0x8fde02a1, "netif_carrier_off" },
	{ 0x31c8e42, "RtmpOSNetDevDetach" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xcb8bc554, "RTUSBWriteBBPRegister" },
	{ 0xb5cd696c, "RTMPAPIoctlE2PROM" },
	{ 0xbe60a720, "RTUSBCancelPendingIRPs" },
	{ 0x40f37283, "RTMPAPPrivIoctlShow" },
	{ 0xb2f4cba7, "RTUSBReadBBPRegister" },
	{ 0x168c3ad5, "APSendPackets" },
	{ 0xbfe40c9e, "TpcReqTabExit" },
	{ 0x1989876b, "APOverlappingBSSScan" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x2f0e269f, "MlmeInit" },
	{ 0x63426114, "RTMPIoctlGetSiteSurvey" },
	{ 0x46fc2020, "APShutdown" },
	{ 0x41e92619, "__init_waitqueue_head" },
	{ 0xd288284d, "APAutoSelectChannel" },
	{ 0xffd5a395, "default_wake_function" },
	{ 0x28b76d99, "RTThreadDequeueCmd" },
	{ 0x9ef93d78, "CfgInitHook" },
	{ 0x72aa82c6, "param_ops_charp" },
	{ 0x185763ca, "APUpdateAllBeaconFrame" },
	{ 0xff31246c, "RtmpOSFSInfoChange" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xefbfaf4f, "RTMPMaxRssi" },
	{ 0x4a20fd4b, "RTMPIoctlStatistics" },
	{ 0x66b40a1d, "RTMPFreeAdapter" },
	{ 0xf4cc6142, "usb_deregister" },
	{ 0x863fa696, "RtmpOSFileOpen" },
	{ 0x27e1a049, "printk" },
	{ 0x4141f80, "__tracepoint_module_get" },
	{ 0x774cc060, "UAPSD_SP_Close" },
	{ 0x8577ae8c, "MbssKickOutStas" },
	{ 0x807ad178, "RT28XX_IOCTL_MaxRateGet" },
	{ 0xbcda97a7, "RTMPFreeTxRxRingMemory" },
	{ 0x765a15f3, "RTMPAPPrivIoctlSet" },
	{ 0x31c87071, "QBSS_LoadInit" },
	{ 0xb0e75266, "hex_dump" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xf6b73965, "RTMPAllocTxRxRingMemory" },
	{ 0x9505d4a, "RtmpOSTaskCustomize" },
	{ 0xb9ad51a3, "TpcReqTabInit" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0xa90492e3, "NICReadEEPROMParameters" },
	{ 0x9e310f23, "RtmpOSNetDevClose" },
	{ 0x747d7183, "UAPSD_UnTagFrame" },
	{ 0x4dd8d8bc, "RTUSBCancelPendingBulkInIRP" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0xadc5045c, "RtmpTimerQInit" },
	{ 0x655aecce, "RtmpOSNetDevAttach" },
	{ 0x3bf7daaf, "RtmpOSNetDevAddrSet" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xe786d401, "MlmeHandler" },
	{ 0xb3f2f19b, "APInitialize" },
	{ 0xff7ef514, "module_put" },
	{ 0x2cbc2d41, "netif_device_attach" },
	{ 0x89477b16, "RTUSBReadMACRegister" },
	{ 0x5dcf41f2, "os_free_mem" },
	{ 0x3a81a795, "netif_device_detach" },
	{ 0x8858a5e5, "usb_get_dev" },
	{ 0x1aca63dc, "RTMPIoctlQueryBaTable" },
	{ 0x5b78c10, "RTMPDeQueuePacket" },
	{ 0xb368ec89, "_raw_spin_unlock_bh" },
	{ 0x19211211, "usb_put_dev" },
	{ 0x7632ce61, "RtmpTimerQThread" },
	{ 0x1000e51, "schedule" },
	{ 0x35300cb4, "UserCfgInit" },
	{ 0x8c9202f6, "RTUSBKickBulkOut" },
	{ 0x778e0825, "AsicLockChannel" },
	{ 0xd39a5e30, "RT28XXDMADisable" },
	{ 0x94dea09a, "kmem_cache_alloc_trace" },
	{ 0xfe57e90, "RTMPCancelTimer" },
	{ 0x36eb4f41, "NICLoadRateSwitchingParams" },
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0xff77fe7d, "RtmpTimerQExit" },
	{ 0x13f99917, "SetCommonHT" },
	{ 0xd2965f6f, "kthread_should_stop" },
	{ 0xe0dd5133, "RtmpRaDevCtrlInit" },
	{ 0x3fdacc6f, "add_wait_queue" },
	{ 0x60f1082e, "RtmpRaDevCtrlExit" },
	{ 0xda0ea564, "MeasureReqTabExit" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xd5e1c1c, "RtmpOSTaskAttach" },
	{ 0x757d9021, "RTDebugLevel" },
	{ 0x801678, "flush_scheduled_work" },
	{ 0x32f80ea9, "prepare_to_wait" },
	{ 0x6940d698, "RTMPSetProfileParameters" },
	{ 0x6e778b01, "MlmeHalt" },
	{ 0xccb6d102, "RTUSBCancelPendingBulkOutIRP" },
	{ 0xd74f18a8, "usb_register_driver" },
	{ 0xb5dc75db, "MeasureReqTabInit" },
	{ 0xf83178bd, "finish_wait" },
	{ 0x69908b5c, "RtmpOSNetDevCreate" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xc2d360e4, "NICLoadFirmware" },
	{ 0x78963f2e, "__netif_schedule" },
	{ 0xae923a8d, "RTEnqueueInternalCmd" },
	{ 0xf6c00c74, "RTMPFreeNdisPacket" },
	{ 0x36881590, "RTMPAPIoctlBBP" },
	{ 0x6fa67456, "RTMPAPSetInformation" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0xa2c22203, "RtmpOSTaskInit" },
	{ 0x3a2fd355, "dev_get_drvdata" },
	{ 0x1ec2d483, "RTMPAPIoctlMAC" },
	{ 0xe914e41e, "strcpy" },
	{ 0x79a1f03a, "APStartUp" },
	{ 0x8cda43ec, "RTMPAllocAdapterBlock" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rt3070ap,rtutil3070ap";

MODULE_ALIAS("usb:v148Fp3070d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v148Fp3071d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v148Fp3072d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p3820d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p871Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p822Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p871Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p822Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p003Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0042d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0048d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0047d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14B2p3C12d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18C5p0012d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083Ap7511d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApA701d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApA702d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9703d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9705d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9706d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9707d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9708d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9709d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3273d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3305d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1044p800Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2019pAB25d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07B8p3070d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07B8p3071d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07B8p3072d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v7392p7711d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1A32p0304d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1EDAp2310d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C0Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C0Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C0Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C0Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C16d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1D4Dp000Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1D4Dp000Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5A57p5257d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5A57p0283d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp0945d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp0947d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp0948d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v203Dp1480d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v20B8p8888d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B05p1784d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v203Dp14A9d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p899Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p3870d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p870Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p6899d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p3822d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p3871d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p871Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p822Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p3821d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DB0p821Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5A57p0282d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApA703d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3307d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3321d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07FAp7712d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0789p0166d*dc*dsc*dp*ic*isc*ip*");
