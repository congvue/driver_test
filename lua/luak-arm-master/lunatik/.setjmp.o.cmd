cmd_/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o := arm-fsl-linux-gnueabi-gcc -Wp,-MD,/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/.setjmp.o.d  -nostdinc -isystem /home/wangyuanfei/opt/Embedsky/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/tq-linaro-toolchain/bin/../lib/gcc/arm-fsl-linux-gnueabi/4.6.2/include -I/home/wangyuanfei/wangyuanfei/anpark/linux-3.0.35/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mx6/include -Iarch/arm/plat-mxc/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float      -DMODULE  -c -o /home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o /home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.S

source_/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o := /home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.S

deps_/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o := \
  /home/wangyuanfei/wangyuanfei/anpark/linux-3.0.35/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/asmmacros.h \

/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o: $(deps_/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o)

$(deps_/home/wangyuanfei/data/driver_perchery/lua/luak-arm-master/lunatik/setjmp.o):
