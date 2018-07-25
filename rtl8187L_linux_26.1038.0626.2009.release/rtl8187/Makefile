NIC_SELECT = RTL8187L

CC = gcc
KVER  := $(shell uname -r)
MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/$(NIC_SELECT)
KSRC := /lib/modules/$(KVER)/build
PWD = $(shell pwd)
INSTALL_PREFIX :=

ifneq ($(shell uname -r|cut -d. -f1,2), 2.4)

#EXTRA_CFLAGS += -DCONFIG_IEEE80211_NOWEP=y
#EXTRA_CFLAGS += -DCONFIG_RTL8180_IOMAP
EXTRA_CFLAGS += -I$(TOPDIR)/drivers/net/wireless
EXTRA_CFLAGS += -std=gnu89
EXTRA_CFLAGS += -O2
EXTRA_CFLAGS += -mhard-float -DCONFIG_FORCE_HARD_FLOAT=y
EXTRA_CFLAGS += -DPOLLING_METHOD_FOR_RADIO
EXTRA_CFLAGS += -DCONFIG_PM
EXTRA_CFLAGS += -DCONFIG_BIG_DATA_NOSCAN
EXTRA_CFLAGS += -DLED
#EXTRA_CFLAGS += -DLED_SHIN
EXTRA_CFLAGS += -DRATE_ADAPTIVE
EXTRA_CFLAGS += -DDIG
#EXTRA_CFLAGS += -DSW_ANTE_DIVERSITY

rtl8187-objs := r8187_core.o r8180_93cx6.o r8180_wx.o r8180_rtl8225.o r8180_rtl8225z2.o r8187_led.o r8180_pm.o r8180_dm.o


obj-m := rtl8187.o



all: modules

modules:
	$(MAKE) -C $(KSRC) M=$(PWD) CC=$(CC) modules

install: modules
	install -p -m 644 rtl8187.ko $(MODDESTDIR)
	depmod -a

uninstall:
	$(shell [ -d $(MODDESTDIR) ] && rm -fr $(MODDESTDIR))
	depmod -a
clean:
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -fr .tmp_versions
	rm -fr Module.symvers
	rm -fr modules.order
	rm -fr Module.markers
	rm -rf tags


else #2.4kernel


LD := ld
KSRC := /lib/modules/$(KVER)/build
CONFIG_FILE := $(KSRC)/include/linux/autoconf.h
CFLAGS += -DLINUX -D__KERNEL__ -DMODULE -O2 -pipe -Wall
CFLAGS += -I$(KSRC)/include -I.
CFLAGS += -DMODVERSIONS -DEXPORT_SYMTAB -include $(KSRC)/include/linux/modversions.h
CFLAGS += -DPOLLING_METHOD_FOR_RADIO
CFLAGS += -DCONFIG_BIG_DATA_NOSCAN
CFLAGS += -DLED
#CFLAGS += -DLED_SHIN
CFLAGS += -DRATE_ADAPTIVE
CFLAGS += -DDIG
#CFLAGS += -DSW_ANTE_DIVERSITY

SMP := $(shell $(CC) $(CFLAGS) -E -dM $(CONFIG_FILE) | \
   grep CONFIG_SMP | awk '{print $$3}')
ifneq ($(SMP),1)
   SMP := 0
endif
ifeq ($(SMP),1)
   CFLAGS    += -D__SMP__
endif
BJS := ${patsubst %.c, %.o, ${wildcard *.c}}

all:rtl8187.o
rtl8187.o: \
	r8187_core.o \
	r8180_93cx6.o \
	r8180_wx.o \
	r8180_rtl8225.o \
	r8180_rtl8225z2.o \
	r8187_led.o \
	r8180_dm.o
	$(LD) -r $^ -o $@
install:rtl8187.o
	install -p -m 644 rtl8187.o  $(MODDESTDIR)
	@depmod -a

uninstall:
	$(shell [ -d $(MODDESTDIR) ] && rm -fr $(MODDESTDIR))
	@depmod -a

clean:
	rm -f *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -rf $(PWD)/tmp
	rm -rf *~
	rm -fr Module.symvers
	rm -fr modules.order
	rm -fr Module.markers
	rm -rf tags

endif
