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

all: buildroot

buildroot: directory download
# its not clear, some testing have to included yet (directorys etc.)
# bevor we start wie have to unpack the kernel if not allready unpacked
	cd $(BUILDDIR); \
	if [ ! -d linux-$(KERNEL) ]; then \
	    tar xjfv $(ARCHIVDIR)/linux-$(KERNEL).tar.bz2; \
	    ln -s linux-$(KERNEL) linux; \
	fi 
# next we unpack buildroot an make symbolic link to the global downloaddir
	cd $(BUILDDIR); \
	test -d $(BUILDDIR)/buildroot || tar xjfv $(ARCHIVDIR)/buildroot-$(BUILDROOTDAY).tar.bz2; \
	rm -rf $(BUILDDIR)/buildroot/dl
	ln -s $(ARCHIVDIR)/dl $(BUILDDIR)/buildroot/dl
# copy the buildroot config to the buildroot dir 
	cd $(BUILDDIR)/buildroot; \
	if [ ! -f .patched ]; then \
	    patch -p1 < $(PATCHDIR)/buildroot.patch001; \
	    touch .patched; \
	    cp $(ACTDIR)/configs/buildroot.config $(BUILDDIR)/buildroot/.config; \
	    cd $(BUILDDIR)/buildroot && make oldconfig && make BR2_STAGING_DIR=$(CCDIR)/mipsel-linux; \
	fi
#	echo Oldconfig erstellt; \
#	rm -rf $(BUILDDIR)/buildroot/toolchain_build_mipsel/linux; \
#	mkdir $(BUILDDIR)/buildroot/toolchain_build_mipsel; \
#	ln -s $(BUILDIR)/linux-$(KERNEL) $(BUILDDIR)/buildroot/toolchain_build_mipsel/linux; \
#	make BR2_STAGING_DIR=$(CCDIR)/mipsel-linux
	
	

directory:
	test -d $(BUILDDIR) || mkdir $(BUILDDIR)
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
	

