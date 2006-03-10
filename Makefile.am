# main Makefile for mct-image builds
@SET_MAKE@

# prefix is default $HOME/mct-build, comes from configure.ac
prefix=@prefix@

ACTDIR:= $(shell pwd)
KERNEL=2.4.32
BUILDROOTDAY=20051202
ARCHIVDIR=${prefix}/archives
BUILDDIR=${prefix}/build
CCDIR=${prefix}/cdk
PATCHDIR=${ACTDIR}/patches
KERNELPATCH=kernel_mct.patch\
            kernel_squashfs.patch\
	    kernel_mppe-mppc.patch\
	    kernel_cryptoloop.patch\
	    kernel_vserver.patch\
	    kernel_grsecurity.patch\
	    kernel_vserver-mips-syscall.patch\
	    kernel_nfs-maxblksize.patch

all: buildroot

buildroot: directory download
# its not clear, some testing have to included yet (directorys etc.)
# bevor we start we have to unpack the kernel if not allready unpacked
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
# next we unpack buildroot an make symbolic link to the global downloaddir
	cd $(BUILDDIR); \
	test -d $(BUILDDIR)/buildroot || tar xjfv $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2; \
	rm -rf $(BUILDDIR)/buildroot/dl
	ln -s $(ARCHIVDIR)/dl $(BUILDDIR)/buildroot/dl
# copy the buildroot config to the buildroot dir 
	cd $(BUILDDIR)/buildroot; \
	if [ ! -f .patched ]; then \
	    patch -p1 < $(PATCHDIR)/buildroot.patch; \
	    touch .patched; \
	    cp $(ACTDIR)/configs/buildroot.config $(BUILDDIR)/buildroot/.config; \
	fi
	cd $(BUILDDIR)/buildroot && make oldconfig && make BR2_STAGING_DIR=$(CCDIR)/mipsel-linux

directory:
	test -d $(BUILDDIR) || mkdir $(BUILDDIR)
	test -d $(ARCHIVDIR)/dl || mkdir $(ARCHIVDIR)/dl
	test -d $(CCDIR) || mkdir $(CCDIR)

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
	

