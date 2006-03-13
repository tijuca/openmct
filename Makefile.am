# main Makefile for mct-image builds
@SET_MAKE@
SUBDIRS = tools progs

# prefix is default $HOME/mct-build, comes from configure.ac
prefix=@prefix@
hostprefix=
ACTDIR:= $(shell pwd)
KERNEL=2.4.32
BUILDROOTDAY=20051202
ARCHIVDIR=${prefix}/archives
BUILDDIR=${prefix}/build
CCDIR=${prefix}/cdk/@target@/bin
DEPDIR=${prefix}/cdk/.deps
PATCHDIR=${ACTDIR}/patches
KERNELPATCH=kernel_mct.patch\
            kernel_squashfs.patch\
	    kernel_mppe-mppc.patch\
	    kernel_cryptoloop.patch\
	    kernel_vserver.patch\
	    kernel_grsecurity.patch\
	    kernel_vserver-mips-syscall.patch\
	    kernel_nfs-maxblksize.patch

PATH := ${CCDIR}:$(PATH)

# $(targetprefix) is now not yet implemented!
# still looking for the right CFLAGS and LDFLAGS

BUILDENV := \
	AR=$(target)-ar \
	AS=$(target)-as \
	CC=$(target)-gcc \
	CXX=$(target)-g++ \
	NM=$(target)-nm \
	RANLIB=$(target)-ranlib \
	CFLAGS="$(TARGET_CFLAGS)" \
	CXXFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	PKG_CONFIG_PATH=$(targetprefix)/lib/pkgconfig


all: prepare buildroot kernel tools progs

prepare: directory
	cd $(BUILDDIR); \
	if [ ! -d linux-$(KERNEL) ]; then \
	    tar xjfv $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2; \
	    ln -s linux-$(KERNEL) linux; \
	fi
	cd $(BUILDDIR)/linux-$(KERNEL); \
	if [ ! -f .patched ]; then \
	    for i in $(KERNELPATCH); do \
		patch -p1 < $(PATCHDIR)/$$i; \
	    done; \
	    touch .patched; \
	fi
	cd $(BUILDDIR); \
	test -d $(BUILDDIR)/buildroot || tar xjfv $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2; \
	rm -rf $(BUILDDIR)/buildroot/dl
	ln -s $(ARCHIVDIR)/dl $(BUILDDIR)/buildroot/dl

buildroot: prepare
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    cd $(BUILDDIR)/buildroot; \
		if [ ! -f .patched ]; then \
		patch -p1 < $(PATCHDIR)/buildroot.patch; \
		touch .patched; \
		cp $(ACTDIR)/configs/buildroot.config $(BUILDDIR)/buildroot/.config; \
	    fi; \
	    cd $(BUILDDIR)/buildroot && $(MAKE) oldconfig && $(MAKE) BR2_STAGING_DIR=$(CCDIR)/mipsel-linux; \
	    touch $(DEPDIR)/$@; \
	fi

kernel: buildroot
	if [ ! -f $(DEPDIR)/$@ ]; then \
	    cd $(BUILDDIR)/linux-$(KERNEL) && \
	    $(BUILDENV) \
	    cp $(ACTDIR)/configs/kernel.config $(BUILDDIR)/linux-$(KERNEL)/.config; \
	    $(MAKE) oldconfig && \
	    $(MAKE) vmlinux CROSS_COMPILE=$(target)-; \
	    touch $(DEPDIR)/$@; \
	fi

tools:
	cd tools && \
	$(MAKE)

progs:
	cd progs && \
	$(MAKE)

directory: download
	test -d $(BUILDDIR) || mkdir $(BUILDDIR)
	test -d $(ARCHIVDIR)/dl || mkdir $(ARCHIVDIR)/dl
	test -d $(CCDIR) || mkdir $(CCDIR)
	test -d $(CCDIR)/.deps || mkdir $(CCDIR)/.deps
#	test -d ${prefix}/cdk/mct-fs || mkdir ${prefix}/cdk/mct-fs

download: $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2 \
          $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2

$(ARCHIVDIR)/linux-$(KERNEL).tar.bz2:
	if [ ! -f $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2 ]; then \
	    wget -c -P $(ARCHIVDIR) http://www.de.kernel.org/pub/linux/kernel/v2.4/linux-$(KERNEL).tar.bz2; \
	fi


$(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2:
	if [ ! -f $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2 ]; then \
	    wget -c -P $(ARCHIVDIR) http://buildroot.uclibc.org/downloads/snapshots/buildroot-$(BUILDROOTDAY).tar.bz2; \
	fi
	

