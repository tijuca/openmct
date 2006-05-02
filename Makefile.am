# main Makefile for mct-image builds
@SET_MAKE@
SUBDIRS = tools progs
VPATH=@srcdir@

AUTOMAKE_OPTION = foreign
# ncurses version
ncursesver=5.5
# gdbm version
gdbmver=1.8.3

# prefix is default $HOME/openmct-build, comes from configure.ac
prefix=@prefix@
ACTDIR:= $(shell pwd)
KERNEL=2.4.32
#BUILDROOTDAY=20051202
BUILDROOTDAY=0.9.27
ARCHIVDIR=${prefix}/archives
BUILDDIR=${prefix}/build
CDK=${prefix}/cdk
CCDIR=$(CDK)/@target@
DEPDIR=$(CDK)/.deps
PATCHDIR=${ACTDIR}/patches
KERNELPATCH=kernel_mct.patch\
            kernel_squashfs.patch\
	    kernel_mppe-mppc.patch\
	    kernel_cryptoloop.patch\
	    kernel_vserver.patch\
	    kernel_grsecurity.patch\
	    kernel_vserver-mips-syscall.patch\
	    kernel_nfs-maxblksize.patch\
	    kernel_usb-wlan.patch

PATH := ${CCDIR}/bin:$(PATH)

# $(targetprefix) is now not yet implemented!
# still looking for the right CFLAGS and LDFLAGS

BUILDENV := \
	AR=$(target)-ar \
	AS=$(target)-as \
	CC=$(target)-gcc \
	CXX=$(target)-g++ \
	NM=$(target)-nm \
	RANLIB=$(target)-ranlib \
	STRIP=$(target)-sstrip \
	CFLAGS="$(TARGET_CFLAGS)" \
	CXXFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	PKG_CONFIG_PATH=$(targetprefix)/lib/pkgconfig


all: prepare buildroot kernel ncurses basic-fs \
     libs \
     tools \
     system \
     network

prepare: directory
# prepare linuxkernel
	cd $(BUILDDIR); \
	if [ ! -d linux-$(KERNEL) ]; then \
	    tar xjfv $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2; \
	    ln -s linux-$(KERNEL) linux; \
	fi
	cd $(BUILDDIR)/linux-$(KERNEL); \
	if [ ! -f .patched ]; then \
	    for i in $(KERNELPATCH); do \
		echo ; \
		echo "patching with $$i"; \
		echo ; \
		patch -p1 < $(PATCHDIR)/$$i; \
	    done; \
	    touch .patched; \
	fi
# prepare buildroot
	cd $(BUILDDIR); \
	test -d $(BUILDDIR)/buildroot || tar xjfv $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2; \
	find -type d -name ".svn" -print0 | xargs -0 rm -rf; \
	rm -rf $(BUILDDIR)/buildroot/dl
	ln -s $(ARCHIVDIR)/dl $(BUILDDIR)/buildroot/dl
# prepare ncurses
	cd $(BUILDDIR); \
	test -d $(BUILDDIR)/ncurses-$(ncursesver) || tar xzvf $(ARCHIVDIR)/ncurses-$(ncursesver).tar.gz

buildroot: prepare
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    cd $(BUILDDIR)/buildroot; \
	    if [ ! -f .patched ]; then \
		patch -p1 < $(PATCHDIR)/buildroot.patch; \
		touch .patched; \
	    fi; \
	    cp $(ACTDIR)/configs/buildroot.config $(BUILDDIR)/buildroot/.config; \
	    cd $(BUILDDIR)/buildroot && \
	    $(MAKE) oldconfig && \
	    $(MAKE) BR2_STAGING_DIR="$(CDK)/mipsel-linux" && \
	    touch $(DEPDIR)/$@; \
	fi


kernel: buildroot
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    cd $(BUILDDIR)/linux-$(KERNEL) && \
	    $(BUILDENV) \
	    cp $(ACTDIR)/configs/kernel.config $(BUILDDIR)/linux-$(KERNEL)/.config; \
	    $(MAKE) oldconfig && \
	    $(MAKE) dep && \
	    $(MAKE) vmlinux CROSS_COMPILE=$(target)-; \
	    cp $(BUILDDIR)/linux-$(KERNEL)/include/linux/version.h $(CCDIR)/include; \
	    touch $(DEPDIR)/$@; \
	fi

ncurses:
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    cd $(BUILDDIR)/ncurses-$(ncursesver); \
		./configure --build=$(build) \
	    	--host=$(target) \
	    	--prefix=$(CCDIR) \
	    	--with-shared \
	    	--disable-ext-funcs \
	    	--with-progs \
	    	$(BUILDENV) &&\
	    	$(MAKE) && \
	    	$(MAKE) install.includes && \
	    	$(MAKE) install.libs && \
	    	$(MAKE) install.man; \
	    touch $(DEPDIR)/$@; \
	fi

basic-fs:
	if [ -d $(CDK)/mct-fs/ ]; then \
	    rm -rf $(CDK)/mct-fs/*; \
	else \
	    mkdir $(CDK)/mct-fs/; \
	fi
	cp -R $(BUILDDIR)/buildroot/build_mipsel/root/* $(CDK)/mct-fs
	rm -rf $(CDK)/mct-fs/etc \
	       $(CDK)/mct-fs/home \
	       $(CDK)/mct-fs/var \
	       $(CDK)/mct-fs/opt
	svn export --force --quiet fs/ $(CDK)/mct-fs
	mkdir $(CDK)/mct-fs/usr/home \
	      $(CDK)/mct-fs/usr/var \
	      $(CDK)/mct-fs/usr/vservers
	ln -s $(CDK)/mct-fs/usr/home $(CDK)/mct-fs/home
	ln -s $(CDK)/mct-fs/usr/var $(CDK)/mct-fs/var
	ln -s $(CDK)/mct-fs/usr/vservers $(CDK)/mct-fs/vservers
	touch $(DEPDIR)/$@

#######################################
#     libs
#

libs: gdbm

gdbm:
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    test -f $(ARCHIVDIR)/gdbm-$(gdbmver).tar.gz || wget ftp://ftp.gnu.org/gnu/gdbm/gdbm-$(gdbmver).tar.gz; \
	    cd $(BUILDDIR); \
	    test -d $(BUILDDIR)/gdbm-$(gdbmver) || tar xzvf $(ARCHIVDIR)/gdbm-$(gdbmver).tar.gz; \
	    cd $(BUILDDIR)/gdbm-$(gdbmver) && \
		$(BUILDENV) \
		./configure \
		    --prefix=$(prefix) \
		    --host=$(target) \
		    --prefix=$(CCDIR) && \
	    $(MAKE) install BINOWN=$(USER) BINGRP=$(USER) && \
	    touch $(DEPDIR)/$@; \
	fi


tools:
	cd tools && \
	    $(MAKE) all

system:
	$(MAKE) -C progs/ system

network:
	$(MAKE) -C progs/ network


directory: download
	test -d $(BUILDDIR) || mkdir $(BUILDDIR)
	test -d $(ARCHIVDIR)/dl || mkdir $(ARCHIVDIR)/dl
	test -d $(CCDIR)/bin || mkdir -p $(CCDIR)/bin
	test -d $(DEPDIR) || mkdir -p $(DEPDIR)
	test -d ${prefix}/cdk/mct-fs || mkdir ${prefix}/cdk/mct-fs

download: $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2 \
          $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2 \
	  $(ARCHIVDIR)/ncurses-$(ncursesver).tar.gz

$(ARCHIVDIR)/linux-$(KERNEL).tar.bz2:
	if [ ! -f $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2 ]; then \
	    wget -c -P $(ARCHIVDIR) http://www.de.kernel.org/pub/linux/kernel/v2.4/linux-$(KERNEL).tar.bz2; \
	fi

$(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2:
	if [ ! -f $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2 ]; then \
	    wget -c -P $(ARCHIVDIR) http://buildroot.uclibc.org/downloads/old/buildroot-$(BUILDROOTDAY).tar.bz2; \
	fi

$(ARCHIVDIR)/ncurses-$(ncursesver).tar.gz:
	if [ ! -f $(ARCHIVDIR)/ncurses-$(ncursesver).tar.gz ]; then \
	    wget -c -P $(ARCHIVDIR) ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-$(ncursesver).tar.gz; \
	fi
