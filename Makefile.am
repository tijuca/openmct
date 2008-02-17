### Makefile for Toolchain for Mipsel with uClibc
# the Basis Rules taken from the freewrt projekt http://freewrt.org/trac
#
# 1. creatingsubdirectories
# 2. extract the kernel und patch him
# 3. installing the header from uClibc
# 4. build Binutils
# 5. build Bootstrap GCC
# 6. build and install uClibc
# 7. build Final GCC
# 8. optional GDB
# 9. build ccache

@SET_MAKE@
#SUBDIRS = tools progs
VPATH=@srcdir@

AUTOMAKE_OPTION = foreign

KERNEL=2.4.35.3
BINUTILS=2.16.1
CCACHE=2.4
GCC=3.3.6
GDB=6.3
GLIBC=2.3.6
UCLIBC=0.9.28

# these are taken from tuxbox, fortunly not nessesary or used
TARGET_CFLAGS= -mmultiple -mstring -meabi -pipe -Os
TARGET_CXXFLAGS=$(TARGET_CFLAGS)
#TARGET_LDFLAGS= -s

TOPDIR := $(shell pwd)
BUILDDIR= $(TOPDIR)/toolchain_build_mipsel
PATCHDIR= $(TOPDIR)/patches
DOWNLOAD= @prefix@/archives
CDK     = @cdk@
CDKROOT = @rootfs@
TFTPBOOT= @tftpboot@
PREFIX  = $(CDK)
CONFIG  = $(TOPDIR)/configs
DEPS    = $(TOPDIR)/.deps
#PATH:=$(PREFIX)/bin:$(PATH)

TARGET=mipsel-linux-uclibc

MKSQUASHFS=$(PREFIX)/bin/mksquashfs
MKJFFS2=$(PREFIX)/bin/mkfs.jffs2
MCTMODIFY=$(PREFIX)/bin/mct_modify

BUILDENV:=AR=$(TARGET)-ar\
          AS=$(TARGET)-ar\
          GCC=$(TARGET)-gcc\
          CXX=$(TARGET)-g++\
          NM=$(TARGET)-nm\
          RANLIB=$(TARGET)-ranlib\
          CFLAGS="$(TARGET_CFLAGS)"\
          CXXFLAGS="$(TARGET_CXXFLAGS)"\
          LDFLAGS="$(TARGET_LDFLAGS)"\
          PKG_CONFIG_PATH=$(CDKROOT)/lib/pkgconfig

include $(TOPDIR)/makefiles/basics.mk
include $(TOPDIR)/makefiles/cdk.mk


########################################################################################
# kernel

kernel24:
	@echo -e "\033[1;33mbuild kernel for @device@ \033[0m ..."
	test -f $(DEPS)/kernel-header || $(MAKE) kernel-header
#	cp $(CONFIG)/kernel-$(KERNEL).config $(BUILDDIR)/linux-$(KERNEL)/.config
	cp $(CONFIG)/kernel-@device@.config $(BUILDDIR)/linux-$(KERNEL)/.config
	cd $(BUILDDIR)/linux-$(KERNEL) &&\
	    $(MAKE) ARCH=mips oldconfig include/linux/version.h &&\
	    PATH=$(PREFIX)/bin:$(PREFIX)/bin-ccache:$(PATH) $(BUILDENV) \
	    $(MAKE) depend &&\
	    $(MAKE) CROSS_COMPILE=$(CDK)/bin/$(TARGET)- vmlinux modules
	cd $(BUILDDIR)/linux-$(KERNEL) &&\
	    $(MAKE) ARCH=mips \
	    PATH=$(PREFIX)/bin:$(PREFIX)/bin-ccache:$(PATH) $(BUILDENV) \
	    DEPMOD=/bin/true \
	    CROSS_COMPILE=$(CDK)/bin-ccache/$(TARGET)- INSTALL_MOD_PATH=$(CDKROOT) modules_install
	
	gzip -9 -c $(BUILDDIR)/linux-$(KERNEL)/vmlinux.bin > $(CDKROOT)/vmlinux.bin.gz
	cp $(BUILDDIR)/linux-$(KERNEL)/System.map $(CDKROOT)
	@echo Kernelsize: `ls -la $(CDKROOT)/vmlinux.bin.gz | cut -f5 -d " "`
	    
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild kernel \033[0m... \033[1;32mDone!\033[0m"



$(DOWNLOAD)/mtd_20050122:
	test -f $(DOWNLOAD)/mtd_20050122.orig.tar.gz || \
	wget http://ftp.debian.org/debian/pool/main/m/mtd/mtd_20050122.orig.tar.gz -P $(DOWNLOAD)

##########################################################################################
# imagebuilding

image:
	@echo -e "\033[1;33mbuild image for @device@\033[0m ..."
#if DEBUG
#	@echo DEBUG enabeld
#else
#	@echo DEBUG disabeld
#endif

etc: mkjffs2
	@echo -e "\033[1;33mbuild rootfs/etc\033[0m ..."
	svn export --force fs/etc $(CDKROOT)/etc
	echo "Welcome to OpenMCT $(rev) from $(shell date)!" > $(CDKROOT)/etc/motd
	echo "Builded from $(SVN_URL)" >> $(CDKROOT)/etc/motd
	echo "@device@" > $(CDKROOT)/etc/hostname
	$(MKJFFS2) -d $(CDKROOT)/etc -o $(prefix)/etc.jffs2 -l -p 65128
	

rootfs:
	@echo -e "\033[1;33mbuild rootfs /\033[0m ..."
	$(MKSQUASHFS) $(CDKROOT) $(prefix)/root.fs -le -all-root -ef $(CDKROOT)/etc/


##########################################################################################
# cleaning

coreclean: kernel-clean uclibc-clean binutils-clean coregcc-clean gdb-clean
	rm -rf $(BUILDDIR)
	rm -rf $(CDK)
	rm -rf $(CDKROOT)
	rm -rf $(TOPDIR)/tftpboot

kernel-clean:
	rm -f $(DEPS)/kernel-header*
	rm -rf $(BUILDDIR)/linux*

uclibc-clean:
	rm -f $(DEPS)/uclibc*
	rm -rf $(BUILDDIR)/uClibc*

binutils-clean:
	rm -f $(DEPS)/binutils
	rm -rf $(BUILDDIR)/binutils*

coregcc-clean:
	rm -f $(DEPS)/coregcc
	rm -rf $(BUILDDIR)/gcc*

gdb-clean:
	rm -f $(DEPS)/gdb
	rm -rf $(BUILDDIR)/gdb*

