cmd_/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o := /opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc -Wp,-MD,/home/launch/samba/wangyuanfei/data/dirver/cw_llc/.llc-msg.o.d  -nostdinc -isystem /opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/lib/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/4.9.1/include -I/home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include -Iarch/arm/include/generated  -Iinclude -I/home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I/home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi -Iinclude/generated/uapi -include /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -fno-dwarf2-cfi-asm -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -funwind-tables -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(llc_msg)"  -D"KBUILD_MODNAME=KBUILD_STR(cw_llc)" -c -o /home/launch/samba/wangyuanfei/data/dirver/cw_llc/.tmp_llc-msg.o /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.c

source_/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o := /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.c

deps_/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o := \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/cohda/llc/llc.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/cohda/llc/llc-api.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-internal.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/linux/cohda/llc/llc.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/posix_types.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/posix_types.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/linux/cohda/llc/llc-api.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/lib/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/4.9.1/include/stdarg.h \
  include/uapi/linux/string.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/string.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/linux/cohda/llc/llc-api.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-netdev.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/net/rx/busy/poll.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/tipc.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/cgroup/net/prio.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
    $(wildcard include/config/net/flow/limit.h) \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/pm_qos.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/linkage.h \
  include/linux/stringify.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/bitops.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/irqflags.h \
    $(wildcard include/config/cpu/v7m.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/hwcap.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/hwcap.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/swab.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  include/linux/kern_levels.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/uapi/linux/kernel.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/sysinfo.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/dynamic_debug.h \
  include/linux/errno.h \
  include/uapi/linux/errno.h \
  arch/arm/include/generated/asm/errno.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/errno.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/errno-base.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/div64.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/compiler.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/bug.h \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/arm/lpae.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/opcodes.h \
    $(wildcard include/config/cpu/endian/be32.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/const.h \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  arch/arm/include/generated/asm/current.h \
  include/asm-generic/current.h \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/linux/bug.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/crunch.h) \
    $(wildcard include/config/arm/thumbee.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/linux/spinlock_types.h \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/debug/spinlock.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/atomic.h \
  include/linux/prefetch.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/arm/errata/754327.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/hw_breakpoint.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/cmpxchg.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  arch/arm/include/generated/asm/preempt.h \
  include/asm-generic/preempt.h \
  include/linux/bottom_half.h \
  include/linux/preempt_mask.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/spinlock.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/user/qs.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/no/hz/full/sysidle.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/seqlock.h \
  include/linux/completion.h \
  include/linux/wait.h \
  include/uapi/linux/wait.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  include/uapi/linux/time.h \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/uapi/linux/timex.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/param.h \
  arch/arm/include/generated/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/uapi/asm-generic/param.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/timex.h \
    $(wildcard include/config/arch/multiplatform.h) \
  include/linux/miscdevice.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/major.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/acpi.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/kobject.h \
    $(wildcard include/config/debug/kobject/release.h) \
  include/linux/sysfs.h \
  include/linux/kernfs.h \
  include/linux/err.h \
  include/linux/idr.h \
  include/linux/rbtree.h \
  include/linux/kobject_ns.h \
  include/linux/stat.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/stat.h \
  include/uapi/linux/stat.h \
  include/linux/uidgid.h \
    $(wildcard include/config/user/ns.h) \
  include/linux/highuid.h \
  include/linux/kref.h \
  include/linux/klist.h \
  include/linux/pinctrl/devinfo.h \
  include/linux/pinctrl/consumer.h \
  include/linux/seq_file.h \
  include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/movable/node.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/pinctrl/pinctrl-state.h \
  include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  include/linux/ratelimit.h \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/cma.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/page-flags-layout.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/generated/bounds.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/kuser/helpers.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/glue.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/pgtable-2level-types.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/memory.h \
    $(wildcard include/config/need/mach/memory/h.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/phys/offset.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/virt/to/bus.h) \
  include/linux/sizes.h \
  include/asm-generic/memory_model.h \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/smp.h \
  include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/smp.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/topology.h \
    $(wildcard include/config/arm/cpu/topology.h) \
  include/asm-generic/topology.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/device.h \
    $(wildcard include/config/dmabounce.h) \
    $(wildcard include/config/iommu/api.h) \
    $(wildcard include/config/arm/dma/use/iommu.h) \
    $(wildcard include/config/arch/omap.h) \
  include/linux/pm_wakeup.h \
  include/linux/delay.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/delay.h \
  include/linux/rculist.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/rapidio/dma/engine.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/uio.h \
  include/uapi/linux/uio.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  include/linux/mm.h \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/metag.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
    $(wildcard include/config/have/cmpxchg/double.h) \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  include/uapi/linux/auxvec.h \
  arch/arm/include/generated/asm/auxvec.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/guard.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  include/linux/uprobes.h \
    $(wildcard include/config/uprobes.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/cpu/has/asid.h) \
  include/linux/range.h \
  include/linux/bit_spinlock.h \
  include/linux/shrinker.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/pgtable.h \
    $(wildcard include/config/highpte.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/proc-fns.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/glue-proc.h \
    $(wildcard include/config/cpu/arm7tdmi.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm740t.h) \
    $(wildcard include/config/cpu/arm9tdmi.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm940t.h) \
    $(wildcard include/config/cpu/arm946e.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/mohawk.h) \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/cpu/v6k.h) \
    $(wildcard include/config/cpu/pj4b.h) \
    $(wildcard include/config/cpu/v7.h) \
  include/asm-generic/pgtable-nopud.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/pgtable-hwdef.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/pgtable-2level-hwdef.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/tlbflush.h \
    $(wildcard include/config/smp/on/up.h) \
    $(wildcard include/config/cpu/tlb/v4wt.h) \
    $(wildcard include/config/cpu/tlb/fa.h) \
    $(wildcard include/config/cpu/tlb/v4wbi.h) \
    $(wildcard include/config/cpu/tlb/feroceon.h) \
    $(wildcard include/config/cpu/tlb/v4wb.h) \
    $(wildcard include/config/cpu/tlb/v6.h) \
    $(wildcard include/config/cpu/tlb/v7.h) \
    $(wildcard include/config/arm/errata/720789.h) \
    $(wildcard include/config/arm/errata/798181.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/no/hz/full.h) \
  include/uapi/linux/sched.h \
  include/linux/capability.h \
  include/uapi/linux/capability.h \
  arch/arm/include/generated/asm/cputime.h \
  include/asm-generic/cputime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/asm-generic/cputime_jiffies.h \
  include/linux/sem.h \
  include/uapi/linux/sem.h \
  include/linux/ipc.h \
  include/uapi/linux/ipc.h \
  arch/arm/include/generated/asm/ipcbuf.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/ipcbuf.h \
  arch/arm/include/generated/asm/sembuf.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/sembuf.h \
  include/linux/signal.h \
    $(wildcard include/config/old/sigaction.h) \
  include/uapi/linux/signal.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/signal.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/signal.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/signal-defs.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/sigcontext.h \
  arch/arm/include/generated/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/uapi/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/seccomp/filter.h) \
  include/uapi/linux/seccomp.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  include/uapi/linux/resource.h \
  arch/arm/include/generated/asm/resource.h \
  include/asm-generic/resource.h \
  include/uapi/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  include/linux/key.h \
  include/linux/sysctl.h \
  include/uapi/linux/sysctl.h \
  include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/pgtable-2level.h \
  include/asm-generic/pgtable.h \
    $(wildcard include/config/have/arch/soft/dirty.h) \
    $(wildcard include/config/arch/uses/numa/prot/none.h) \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/debug/tlbflush.h) \
  include/linux/vm_event_item.h \
    $(wildcard include/config/migration.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/scatterlist.h \
    $(wildcard include/config/arm/has/sg/chain.h) \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/io.h \
    $(wildcard include/config/need/mach/io/h.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/pcmcia/soc/common.h) \
    $(wildcard include/config/isa.h) \
    $(wildcard include/config/pccard.h) \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  include/xen/xen.h \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/xen/dom0.h) \
    $(wildcard include/config/xen/pvh.h) \
  include/linux/dynamic_queue_limits.h \
  include/linux/ethtool.h \
  include/linux/compat.h \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  include/uapi/linux/ethtool.h \
  include/linux/if_ether.h \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
    $(wildcard include/config/netfilter/xt/target/trace.h) \
    $(wildcard include/config/nf/tables.h) \
  include/linux/kmemcheck.h \
  include/linux/net.h \
  include/linux/random.h \
    $(wildcard include/config/arch/random.h) \
  include/uapi/linux/random.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/ioctl.h \
  arch/arm/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/irqnr.h \
  include/uapi/linux/irqnr.h \
  include/linux/fcntl.h \
  include/uapi/linux/fcntl.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/uapi/asm/fcntl.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/fcntl.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  include/uapi/linux/net.h \
  include/linux/socket.h \
  arch/arm/include/generated/asm/socket.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/socket.h \
  arch/arm/include/generated/asm/sockios.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/asm-generic/sockios.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/sockios.h \
  include/uapi/linux/socket.h \
  include/linux/textsearch.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/memcg/kmem.h) \
  include/net/checksum.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/uaccess.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/checksum.h \
  include/linux/in6.h \
  include/uapi/linux/in6.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/libc-compat.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/dma-direction.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/dma-mapping.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/xen/hypervisor.h \
  include/asm-generic/dma-mapping-common.h \
  include/linux/netdev_features.h \
  include/net/flow_keys.h \
  include/uapi/linux/if_ether.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/sctp.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/ip/vs.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/sctp.h \
  include/net/netns/dccp.h \
  include/net/netns/netfilter.h \
  include/linux/proc_fs.h \
  include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/fs/xip.h) \
  include/linux/kdev_t.h \
  include/uapi/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  include/linux/path.h \
  include/linux/list_lru.h \
  include/linux/radix-tree.h \
  include/linux/semaphore.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/fiemap.h \
  include/linux/migrate_mode.h \
  include/linux/percpu-rwsem.h \
  include/uapi/linux/fs.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/limits.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/projid.h \
  include/uapi/linux/quota.h \
  include/linux/nfs_fs_i.h \
  include/linux/netfilter.h \
    $(wildcard include/config/nf/nat/needed.h) \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/if.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/hdlc/ioctl.h \
  include/linux/in.h \
  include/uapi/linux/in.h \
  include/uapi/linux/netfilter.h \
  include/net/flow.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
    $(wildcard include/config/ip/nf/target/ulog.h) \
    $(wildcard include/config/bridge/ebt/ulog.h) \
  include/net/netns/nftables.h \
  include/net/netns/xfrm.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/nsproxy.h \
  include/net/dsa.h \
  include/net/netprio_cgroup.h \
  include/linux/cgroup.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/cgroupstats.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/taskstats.h \
  include/linux/prio_heap.h \
  include/linux/xattr.h \
  include/uapi/linux/xattr.h \
  include/linux/percpu-refcount.h \
  include/linux/cgroup_subsys.h \
    $(wildcard include/config/cgroup/debug.h) \
    $(wildcard include/config/cgroup/cpuacct.h) \
    $(wildcard include/config/cgroup/device.h) \
    $(wildcard include/config/cgroup/freezer.h) \
    $(wildcard include/config/cgroup/net/classid.h) \
    $(wildcard include/config/cgroup/perf.h) \
    $(wildcard include/config/cgroup/hugetlb.h) \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  include/linux/vtime.h \
  include/linux/context_tracking_state.h \
  include/linux/static_key.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/hardirq.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/arch/arm/include/asm/irq.h \
    $(wildcard include/config/sparse/irq.h) \
    $(wildcard include/config/multi/irq/handler.h) \
  include/linux/irq_cpustat.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/neighbour.h \
  include/linux/netlink.h \
  include/net/scm.h \
    $(wildcard include/config/security/network.h) \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
    $(wildcard include/config/security/yama.h) \
  include/uapi/linux/netlink.h \
  include/uapi/linux/netdevice.h \
  /home/launch/samba/wangyuanfei/imx6solo_fsl-release-bsp/test_yocto/tmp/work/imx6solosabresd-poky-linux-gnueabi/linux-imx/3.14.28-r0/git/include/uapi/linux/if_packet.h \
  include/linux/if_link.h \
  include/uapi/linux/if_link.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-monitor.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-thread.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-list.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-usb.h \
  include/linux/usb.h \
    $(wildcard include/config/usb/mon.h) \
  include/linux/mod_devicetable.h \
  include/linux/uuid.h \
  include/uapi/linux/uuid.h \
  include/linux/usb/ch9.h \
  include/uapi/linux/usb/ch9.h \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/att/one.h) \
    $(wildcard include/config/att/selfpower.h) \
    $(wildcard include/config/att/wakeup.h) \
    $(wildcard include/config/att/battery.h) \
  include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/irqreturn.h \
  include/linux/pm_runtime.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-firmware.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-spi.h \
  include/linux/spi/spi.h \
    $(wildcard include/config/spi.h) \
  include/linux/kthread.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-device.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/debug-levels.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/linux/cohda/debug.h \
  /home/launch/samba/wangyuanfei/data/dirver/cw_llc/linux/cohda/debug-kernel.h \

/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o: $(deps_/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o)

$(deps_/home/launch/samba/wangyuanfei/data/dirver/cw_llc/llc-msg.o):
