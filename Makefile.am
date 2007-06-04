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

@SET_MAKE@
#SUBDIRS = tools progs
VPATH=@srcdir@

AUTOMAKE_OPTION = foreign

KERNEL=2.4.34.1
BINUTILS=2.16.1
CCACHE=2.4
GCC=3.4.4
GDB=6.3
GLIBC=2.3.6
UCLIBC=0.9.28

SQUASHFS=3.0
LZMA=442

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

cdk:
	test -f $(DEPS)/$@ || \
	    $(MAKE) core; \
	    $(MAKE) tools
	touch $(DEPS)/$@

###################################################################
# core 
#

core: kernel-header uclibc-header binutils coregcc uclibc gcc-final cdk-extra gdb ccache

###################################################################
# helper applications  --> tools 
#

tools: mct_modify mkjffs2 mksquashfs.lzma

prepare: 
# creating .deps dir
	test -d $(DEPS) || mkdir -p $(DEPS)
# creating the temporaer builddir
	test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
# creating the cdk dir
	test -d $(CDK) || mkdir -p $(CDK)
# creating targetdir
	install -c -d $(CDK)/$(TARGET)
	install -c -d $(CDK)/$(TARGET)/include
	install -c -d $(CDK)/$(TARGET)/lib
# creating the tftp boot dir
	install -c -d $(TFTPBOOT)
# building the fs skeleton
cdkroot-skeleton:
	install -c -d $(CDKROOT)/bin
	install -c -d $(CDKROOT)/boot
	install -c -d $(CDKROOT)/dev
	install -c -d $(CDKROOT)/etc
	install -c -d $(CDKROOT)/include
	install -c -d $(CDKROOT)/mnt
	install -c -d $(CDKROOT)/lib
	install -c -d $(CDKROOT)/lib/pkgconfig
	install -c -d $(CDKROOT)/proc
	install -c -d $(CDKROOT)/root
	install -c -d $(CDKROOT)/sbin
	install -c -d $(CDKROOT)/tmp
	install -c -d $(CDKROOT)/var
	install -c -d $(CDKROOT)/var/etc
	install -c -d $(CDK)/$(TARGET)/usr/include
	install -c -d $(CDK)/$(TARGET)/usr/lib
# setting some symlinks
	ln -sf $(CDKROOT)/include                    $(CDK)/$(TARGET)/include
	ln -sf $(CDKROOT)/lib                        $(CDK)/$(TARGET)/lib

kernel-header: prepare $(DOWNLOAD)/linux-$(KERNEL)
	@echo -e "\033[1;33mprepare Kernel \033[0m(kernel-version $(KERNEL)) ..."
	rm -rf $(BUILDDIR)/linux-$(KERNEL)
	test -d $(BUILDDIR)/linux-$(KERNEL) || (bzcat $(DOWNLOAD)/linux-$(KERNEL).tar.bz2 | /bin/tar -C $(BUILDDIR) -xf - && cd $(BUILDDIR)/linux-$(KERNEL) && \
	    echo -e "\npatching 001-mct.patch" && patch -p1 < $(PATCHDIR)/kernel/001-mct.patch &&\
	    echo -e "\npatching 002-squashfs.patch" && patch -p1 < $(PATCHDIR)/kernel/002-squashfs.patch &&\
	    echo -e "\npatching 003-mppe-mppc.patch" && patch -p1 < $(PATCHDIR)/kernel/003-mppe-mppc.patch &&\
	    echo -e "\npatching 005-vserver.patch" && patch -p1 < $(PATCHDIR)/kernel/005-vserver.patch &&\
	    echo -e "\npatching 007-vserver-mips-syscall.patch" && patch -p1 < $(PATCHDIR)/kernel/007-vserver-mips-syscall.patch &&\
	    echo -e "\npatching 008-nfs-maxblksize.patch" && patch -p1 < $(PATCHDIR)/kernel/008-nfs-maxblksize.patch &&\
	    echo -e "\npatching 009-usb-wlan.patch" && patch -p1 < $(PATCHDIR)/kernel/009-usb-wlan.patch &&\
	    echo -e "\npatching 010-usb-camera.patch" && patch -p1 < $(PATCHDIR)/kernel/010-usb-camera.patch &&\
	    echo -e "\npatching 011-lzma.patch" && patch -p1 < $(PATCHDIR)/kernel/011-lzma.patch &&\
	    echo -e "\npatching 012-squashfs-lzma.patch" && patch -p1 < $(PATCHDIR)/kernel/012-squashfs-lzma.patch &&\
	    echo -e "\npatching 013-scsi-idle.patch" && patch -p1 < $(PATCHDIR)/kernel/013-scsi-idle.patch)
	ln -sf $(BUILDDIR)/linux-$(KERNEL) $(BUILDDIR)/linux
	touch $(DEPS)/$@-prepare
	cd $(BUILDDIR) &&\
	cp $(CONFIG)/kernel-$(KERNEL).config $(BUILDDIR)/linux-$(KERNEL)/.config && \
	$(MAKE) -C linux-$(KERNEL) ARCH=mips oldconfig include/linux/version.h 
	touch $(DEPS)/$@
	@echo -e "\033[1;33mprepare Kernel \033[0m... \033[1;32mDone!\033[0m"

uclibc-header: $(DOWNLOAD)/uClibc-$(UCLIBC)
	@echo -e "\033[1;33mprepare uClibc-Header \033[0m(uClibc-version $(UCLIBC)) ..."
	test -d $(BUILDDIR)/linux-$(KERNEL) || $(MAKE) kernel-header
	test -d $(BUILDDIR)/uClibc-$(UCLIBC) || (bzcat $(DOWNLOAD)/uClibc-$(UCLIBC).tar.bz2 | /bin/tar -C $(BUILDDIR) -xf - &&	cd $(BUILDDIR)/uClibc-$(UCLIBC) && install -m444 $(DOWNLOAD)/uClibc-locale-030818.tgz extra/locale/ &&\
	cd $(BUILDDIR)/uClibc-$(UCLIBC) && patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/100-signal_handler_crash_workaround.patch &&\
					   patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/110-mips_pipe.patch &&\
					   patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/120-more_standard_math.patch &&\
					   patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/140-uclibc_do_div_ldso.patch &&\
					   patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/150-portability.patch &&\
					   patch -Np1 -i $(PATCHDIR)/uClibc-$(UCLIBC)/ipv6_missing_sockopts.patch)
	sed -i -e 's,^CROSS=.*,CROSS=$(CDK)/bin/$(TARGET)-,g' $(BUILDDIR)/uClibc-$(UCLIBC)/Rules.mak
	cp $(CONFIG)/uclibc.config $(BUILDDIR)/uClibc-$(UCLIBC)/.config
	sed -i -e 's,^KERNEL_SOURCE=.*,KERNEL_SOURCE=\"$(BUILDDIR)/linux\",g' $(BUILDDIR)/uClibc-$(UCLIBC)/.config
	sed -i -e 's,^.*UCLIBC_HAS_LFS.*,UCLIBC_HAS_LFS=y,g' $(BUILDDIR)/uClibc-$(UCLIBC)/.config
	sed -i -e 's,.*UCLIBC_HAS_WCHAR.*,UCLIBC_HAS_WCHAR=y,g' $(BUILDDIR)/uClibc-$(UCLIBC)/.config
	mkdir -p $(BUILDDIR)/uClibc_dev/usr/include
	mkdir -p $(BUILDDIR)/uClibc_dev/usr/lib
	mkdir -p $(BUILDDIR)/uClibc_dev/lib
	touch $(BUILDDIR)/uClibc-$(UCLIBC)/.configured $(BUILDDIR)/uClibc-$(UCLIBC)/.unpacked
	cd $(BUILDDIR)/uClibc-$(UCLIBC) && $(MAKE) PREFIX=$(BUILDDIR)/uClibc_dev/ \
						DEVEL_PREFIX=/usr/ \
						RUNTIME_PREFIX=$(BUILDDIR)/uClibc_dev/ \
						HOSTCC="gcc" CC="cc" \
						CPU_CFLAGS="-Os -fno-ident -fwrapv -fhonour-copts" \
						pregen install_dev
	touch $(DEPS)/$@
	@echo -e "\033[1;33mprepare uClibc-Header \033[0m... \033[1;32mDone!\033[0m"


binutils: $(DOWNLOAD)/binutils-$(BINUTILS)
	@echo -e "\033[1;33mbuild Binutils \033[0m(binutils-version $(BINUTILS)) ..."
	test -d $(BUILDDIR)/uClibc-$(UCLIBC) || $(MAKE) uclibc-header
	test -d $(BUILDDIR)/binutils-$(BINUTILS) || (bzcat $(DOWNLOAD)/binutils-$(BINUTILS).tar.bz2 | /bin/tar -C $(BUILDDIR) -xf - &&\
	cd $(BUILDDIR)/binutils-$(BINUTILS) && patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/042-ld-Wlcomma.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/100-uclibc-conf.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/110-uclibc-libtool-conf.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/300-001_ld_makefile_patch.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/300-006_better_file_error.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/300-012_check_ldrunpath_length.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/400-mips-ELF_MAXPAGESIZE-4k.patch &&\
					       patch -Np1 -i $(PATCHDIR)/binutils-$(BINUTILS)/702-binutils-skip-comments.patch)
					       
	mkdir -p $(BUILDDIR)/binutils_build && cd $(BUILDDIR)/binutils_build &&\
	        $(BUILDDIR)/binutils-$(BINUTILS)/configure \
	        --prefix=$(PREFIX) \
	        --target=$(TARGET) \
	        --disable-nls \
		--disable-dependency-tracking \
		--disable-libtool-lock \
		--disable-werror &&\
	        $(MAKE) all &&\
	        $(MAKE) install
		
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild binutils \033[0m... \033[1;32mDone!\033[0m"



coregcc: $(DOWNLOAD)/gcc-$(GCC)
	@echo -e "\033[1;33mbuild coregcc \033[0m(gcc-version $(GCC)) ..."
	test -d $(BUILDDIR)/binutils-$(BINUTILS) || $(MAKE) binutils
	test -d $(BUILDDIR)/gcc-$(GCC) || (bzcat $(DOWNLOAD)/gcc-$(GCC).tar.bz2 | /bin/tar -C $(BUILDDIR) -xf - &&\
	cd $(BUILDDIR)/gcc-$(GCC) && patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/100-uclibc-conf.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/200-uclibc-locale.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/300-libstdc++-pic.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/601-gcc34-arm-ldm.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/602-sdk-libstdc++-includes.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/700-pr15068-fix.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/800-arm-bigendian.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/810-arm-bigendian-uclibc.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/830-gcc-bug-num-22167.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/900-use-cp.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/910-mbsd-multi.patch &&\
				     patch -Np1 -i $(PATCHDIR)/gcc-$(GCC)/920-integer-overflow.patch)
	
#adding special info to the compiler			     
	sed -i -e 's,\(version_string.. = "[0-9\.]*\).*\(";\),\1 (OpenMCT)\2,' $(BUILDDIR)/gcc-$(GCC)/gcc/version.c
	sed -i -e 's,\(bug_report_url.. = "\).*\(";\),\1<URL:http://forum.openmct.org>\2,' $(BUILDDIR)/gcc-$(GCC)/gcc/version.c
	
	mkdir -p $(BUILDDIR)/coregcc_build && (cd $(BUILDDIR)/coregcc_build && rm -f config.cache && PATH=$(PREFIX)/bin:$(PATH) \
	    $(BUILDDIR)/gcc-$(GCC)/configure \
	        --prefix=$(PREFIX) \
		--build=$(build) \
		--host=$(host) \
	        --target=$(TARGET) \
		--enable-languages=c \
	        --disable-shared \
		--with-sysroot=$(BUILDDIR)/uClibc_dev/ \
		--disable-__cxa_atexit \
		--enable-target-optspace \
		--with-gnu-ld \
		--disable-nls \
		--disable-depency-tracking \
		--disable-libtool-lock && \
	        PATH=$(PREFIX)/bin:$(PATH) $(MAKE) all-gcc &&\
	        PATH=$(PREFIX)/bin:$(PATH) $(MAKE) install-gcc)
	
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild coregcc \033[0m... \033[1;32mDone!\033[0m"

uclibc:
	@echo -e "\033[1;33mbuild uClibc \033[0m ..."
	test -d $(BUILDDIR)/gcc-$(GCC) || $(MAKE) coregcc
	cd $(BUILDDIR)/uClibc-$(UCLIBC) && $(MAKE) PREFIX= \
						DEVEL_PREFIX=/ \
						RUNTIME_PREFIX=/ \
						HOSTCC="cc" \
						CPU_CFLAGS="-Os -fno-ident -fwrapv -fhonour-copts" \
						all
	touch -c $(BUILDDIR)/uClibc-$(UCLIBC)/lib/libc.a
	cd $(BUILDDIR)/uClibc-$(UCLIBC) && $(MAKE) PREFIX=$(CDK)/ \
				                DEVEL_PREFIX=/ \
						RUNTIME_PREFIX=/ \
						install_runtime
	cd $(BUILDDIR)/uClibc-$(UCLIBC) && $(MAKE) PREFIX=$(CDK)/ \
						DEVEL_PREFIX=/ \
						RUNTIME_PREFIX=/ \
						install_dev
	echo $(UCLIBC) > $(CDK)/uclibc_version
	touch -c $(CDK)/lib/libc.a
	
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild uClibc \033[0m... \033[1;32mDone!\033[0m"

gcc-final:
	@echo -e "\033[1;33mbuild final gcc \033[0m ..."
	test -f $(CDK)/lib/libc.a || $(MAKE) uclibc
	mkdir -p $(BUILDDIR)/gcc-$(GCC)-final
# Important!  Required for limits.h to be fixed.
	rm -rf $(CDK)/$(TARGET)/sys-include
	ln -sf $(CDK)/include $(CDK)/$(TARGET)/sys-include
	rm -rf $(CDK)/$(TARGET)/lib
	ln -sf $(CDK)/lib $(CDK)/$(TARGET)/lib
	(cd $(BUILDDIR)/gcc-$(GCC)-final; rm -f config.cache; PATH=$(PREFIX)/bin:$(PATH) \
	    $(BUILDDIR)/gcc-$(GCC)/configure \
		--prefix=$(CDK) \
		--build=$(build) \
		--host=$(host) \
		--target=$(TARGET) \
		--enable-languages=c,c++ \
		--enable-shared \
		--disable-__cxa_atexit \
		--enable-target-optspace \
		--with-gnu-ld \
		--disable-nls && \
	PATH=$(PREFIX)/bin:$(PATH) $(MAKE) all &&\
	PATH=$(PREFIX)/bin:$(PATH) $(MAKE) install)
	echo $(GCC) > $(CDK)/gcc_version
	
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild final gcc \033[0m... \033[1;32mDone!\033[0m"


cdk-extra:
	@echo -e "\033[1;33mfinal work on the cdk \033[0m ..."
	test -f $(CDK)/bin/$(TARGET)-gcc || $(MAKE) gcc-final
# Set up the symlinks to enable lying about target name.
	set -e; \
	(cd $(CDK) ;\
	        ln -sf $(TARGET) mipsel-linux ;\
	        cd bin ;\
	        for app in $(TARGET)-* ; do \
	                ln -sf $${app} mipsel-linux$${app##$(TARGET)}; \
	        done)
# We do another ugly hack here because the standard behaviour is
# to include a reference to libgcc.so.1 in all binaries. For flash space
# saving, we change the specs file to link in a static libgcc here.
	if [ -f $(CDK)/lib/gcc/$(TARGET)/$(GCC)/specs ] ; then \
	        patch -d $(CDK)/lib/gcc/$(TARGET)/$(GCC)/ -p0 < $(PATCHDIR)/gcc-$(GCC)/static-libgcc.patch.conditional ; \
	fi
	
	touch $(DEPS)/$@
	@echo -e "\033[1;33mfinal work on the cdk \033[0m... \033[1;32mDone!\033[0m"

gdb: $(DOWNLOAD)/gdb-$(GDB)
	@echo -e "\033[1;33mbuild GDB (gdb-version $(GDB)) \033[0m ..."
	test -d $(BUILDDIR)/gdb-$(GDB) || (bzcat $(DOWNLOAD)/gdb-$(GDB).tar.bz2 | /bin/tar -C $(BUILDDIR) -xf - && cd $(BUILDDIR)/gdb-$(GDB) &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/100-uclibc-conf.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/200-uclibc-readline-conf.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/400-mips-coredump.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/500-thread-timeout.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/600-debian_10.selected-frame.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/620-debian_static-thread-db.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/630-debian_24.tracepoint-segv.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/640-debian_dwarf2-frame-signal-unwinder.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/650-debian_vsyscall-gdb-support.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/660-debian_dwarf-cfa-restore.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/680-debian_sim-destdir.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/690-debian_member-field-symtab.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/700-debian_cp-pass-by-reference.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/710-debian_thread-db-multiple-libraries.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/720-debian_static-threads-test.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/730-debian_gdb-fix-tracefork-check.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/740-debian_make-cv-type-crash.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/750-debian_sparc-singlestep.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/760-debian_vsyscall-bfd-close-result.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/770-debian_vfork-done-spelling.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/780-debian_gdbserver-rdynamic.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/790-debian_dwarf2-cfi-warning.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/800-debian_linux-use-underscore-exit.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/810-debian_bfd-no-kylix-crash.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/820-debian_disable-linux-fork-messages.patch &&\
	    patch -Np1 -i $(PATCHDIR)/gdb-$(GDB)/830-arm_gcc4.patch)
	mkdir -p $(BUILDDIR)/gdbclient-$(GDB) &&\
	(cd $(BUILDDIR)/gdbclient-$(GDB) &&\
	    gdb_cv_func_sigsetjmp=yes \
	    $(BUILDDIR)/gdb-$(GDB)/configure \
	    --prefix=$(CDK) \
	    --build=$(build) \
	    --host=$(host) \
	    --target=$(TARGET) \
	    --disable-nls \
	    --without-uiout --disable-gdbmi \
	    --disable-tui --disable-gdbtk --without-x \
	    --without-included-gettext \
	    --enable-threads \
	    --disable-dependency-tracking \
	    --disable-libtool-lock &&\
	    CC='cc' CFLAGS='-O2 -fwrapv -fno-tree-vrp'  &&\
	    $(MAKE) &&\
	strip $(BUILDDIR)/gdbclient-$(GDB)/gdb/gdb &&\
	install $(BUILDDIR)/gdbclient-$(GDB)/gdb/gdb $(CDK)/bin/$(TARGET)-gdb &&\
	cd $(CDK)/bin && ln -sf $(CDK)/bin/$(TARGET)-gdb mipsel-linux-gdb)
	
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild GDB \033[0m... \033[1;32mDone!\033[0m"

ccache: $(DOWNLOAD)/ccache-$(CCACHE)
	@echo -e "\033[1;33mbuild ccache\033[0m ..."
	rm -rf $(BUILDDIR)/ccache-$(CCACHE) && cd $(BUILDDIR) && tar xzf $(DOWNLOAD)/ccache-$(CCACHE).tar.gz
	cd $(BUILDDIR)/ccache-$(CCACHE) &&\
	    rm -rf config.cache; \
	    ./configure \
	    --target=$(host) \
	    --host=$(host) \
	    --build=$(host) \
	    --prefix=$(CDK)/bin \
	    --exec-prefix=$(CDK)/bin \
	    --bindir=$(CDK)/bin \
	    --sbindir=$(CDK)/sbin \
	    --libexecdir=$(CDK)/lib \
	    --sysconfdir=$(CDK)/etc \
	    --datadir=$(CDK)/share \
	    --localstatedir=$(CDK)/var \
	    --mandir=$(CDK)/man \
	    --infodir=$(CDK)/info
	$(MAKE) -C $(BUILDDIR)/ccache-$(CCACHE) all install
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild ccache \033[0m... \033[1;32mDone!\033[0m"

########################################################################################
# kernel

kernel24:
	@echo -e "\033[1;33mbuild kernel for @device@ \033[0m ..."
	test -f $(DEPS)/kernel-header || $(MAKE) kernel-header
#	cp $(CONFIG)/kernel-$(KERNEL).config $(BUILDDIR)/linux-$(KERNEL)/.config
	cp $(CONFIG)/kernel-@device@.config $(BUILDDIR)/linux-$(KERNEL)/.config
	cd $(BUILDDIR)/linux-$(KERNEL) &&\
	    $(MAKE) ARCH=mips oldconfig include/linux/version.h &&\
	    PATH=$(PREFIX)/bin:$(PATH) $(BUILDENV) \
	    $(MAKE) depend &&\
	    $(MAKE) CROSS_COMPILE=$(CDK)/bin/$(TARGET)- vmlinux modules
	cd $(BUILDDIR)/linux-$(KERNEL) &&\
	    $(MAKE) ARCH=mips \
	    PATH=$(PREFIX)/bin:$(PATH) $(BUILDENV) \
	    DEPMOD=/bin/true \
	    CROSS_COMPILE=$(CDK)/bin/$(TARGET)- INSTALL_MOD_PATH=$(CDKROOT) modules_install
	
	gzip -9 -c $(BUILDDIR)/linux-$(KERNEL)/vmlinux.bin > $(CDKROOT)/vmlinux.bin.gz
	cp $(BUILDDIR)/linux-$(KERNEL)/System.map $(CDKROOT)
	@echo Kernelsize: `ls -la $(CDKROOT)/vmlinux.bin.gz | cut -f5 -d " "`
	    
	touch $(DEPS)/$@
	@echo -e "\033[1;33mbuild kernel \033[0m... \033[1;32mDone!\033[0m"

#######################################################################################
# tools

mct_modify:
	if [ ! -f $(DEPS)/$@ ]; then \
	    echo -e "\033[1;33mbuild mct_modify\033[0m ..."; \
	    $(MAKE) -C tools/mct_modify all install; \
	    echo -e "\033[1;33mbuild mct_modfy \033[0m... \033[1;32mDone!\033[0m"; \
	fi
	touch $(DEPS)/$@

mkjffs2:
	if [ ! -f $(DEPS)/$@ ]; then \
	    echo -e "\033[1;33mbuild mkjffs2\033[0m ..."; \
	    $(MAKE) -C tools/mkfs.jffs2 all install; \
	    echo -e "\033[1;33mbuild mkjffs2 \033[0m... \033[1;32mDone!\033[0m"; \
	fi
	touch $(DEPS)/$@

mksquashfs.lzma: $(DOWNLOAD)/squashfs$(SQUASHFS) $(DOWNLOAD)/lzma$(LZMA)
	if [ ! -f $(DEPS)/$@ ]; then \
	    echo -e "\033[1;33mbuild mksquashfs  \033[0m ..."; \
	    rm -rf mksquashfs && mkdir -p mksquashfs; \
	    cd mksquashfs &&\
		/bin/tar xzf $(DOWNLOAD)/squashfs$(SQUASHFS).tar.gz &&\
		patch -Np0 -i $(PATCHDIR)/squashfs/mksquashfs_lzma.diff &&\
		bzcat $(DOWNLOAD)/lzma$(LZMA).tar.bz2 | /bin/tar -xf - &&\
		patch -Np1 -i $(PATCHDIR)/lzma/lzma_zlib-stream.diff; \
	    $(MAKE) -C C/7zip/Compress/LZMA_Lib all; \
	    $(MAKE) -C squashfs3.0/squashfs-tools; \
	    $(INSTALL) -m755 squashfs3.0/squashfs-tools/mksquashfs $(CDK)/bin; \
	    cd .. && rm -rf mksquashfs; \
	    echo -e "\033[1;33mbuild mksquashfs \033[0m... \033[1;32mDone!\033[0m"; \
	fi
	touch $(DEPS)/$@

########################################################################################
# downloads

$(DOWNLOAD)/linux-$(KERNEL):
	test -f $(DOWNLOAD)/linux-$(KERNEL).tar.bz2 || \
	wget http://www.de.kernel.org/pub/linux/kernel/v2.4/linux-$(KERNEL).tar.bz2 -P $(DOWNLOAD) || \
	wget http://www.kernel.org/pub/linux/kernel/v2.4/linux-$(KERNEL).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/binutils-$(BINUTILS):
	test -f $(DOWNLOAD)/binutils-$(BINUTILS).tar.bz2 || \
	wget http://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/gcc-$(GCC):
	test -f $(DOWNLOAD)/gcc-$(GCC).tar.bz2 || \
	wget ftp://ftp.gwdg.de/pub/misc/gcc/releases/gcc-$(GCC)/gcc-$(GCC).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/gcc-core-$(GCC):
	test -f $(DOWNLOAD)/gcc-core-$(GCC).tar.bz2 || \
	wget ftp://ftp.gwdg.de/pub/misc/gcc/releases/gcc-$(GCC)/gcc-core-$(GCC).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/gcc-g++-$(GCC):
	test -f $(DOWNLOAD)/gcc-g++-$(GCC).tar.bz2 || \
	wget ftp://ftp.gwdg.de/pub/misc/gcc/releases/gcc-$(GCC)/gcc-g++-$(GCC).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/gdb-$(GDB):
	test -f $(DOWNLOAD)/gdb-$(GDB).tar.bz2 || \
	wget http://ftp.gnu.org/gnu/gdb/gdb-$(GDB).tar.bz2 -P $(DOWNLOAD) || \
	wget http://cudlug.cudenver.edu/GNU/gnu/gdb/gdb-$(GDB).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/glibc-$(GLIBC):
	test -f $(DOWNLOAD)/glibc-$(GLIBC).tar.bz2 || \
	wget -c --passive-ftp ftp://ftp.gnu.org/gnu/glibc/glibc-$(GLIBC).tar.bz2 -P $(DOWNLOAD)
	test -f $(DOWNLOAD)/glibc-linuxthreads-$(GLIBC).tar.bz2 || \
	wget -c --passive-ftp ftp://ftp.gnu.org/gnu/glibc/glibc-linuxthreads-$(GLIBC).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/uClibc-$(UCLIBC):
	test -f $(DOWNLOAD)/uClibc-$(UCLIBC).tar.bz2 || \
	wget -c --passive-ftp http://uclibc.org/downloads/uClibc-$(UCLIBC).tar.bz2 -P $(DOWNLOAD)
	test -f $(DOWNLOAD)/uClibc-locale-030818.tgz || \
	wget -c --passive-ftp http://uclibc.org/downloads/uClibc-locale-030818.tgz -P $(DOWNLOAD)

# downloading the tools # 
$(DOWNLOAD)/squashfs$(SQUASHFS):
	test -f $(DOWNLOAD)/squashfs$(SQUASHFS).tar.gz || \
	wget http://umn.dl.sourceforge.net/sourceforge/squashfs/squashfs$(SQUASHFS).tar.gz -P $(DOWNLOAD)
	
$(DOWNLOAD)/lzma$(LZMA):
	test -f $(DOWNLOAD)/lzma$(LZMA).tar.bz2 || \
	wget http://heanet.dl.sourceforge.net/sourceforge/sevenzip/lzma$(LZMA).tar.bz2 -P $(DOWNLOAD)
#	wget http://www.7-zip.org/dl/lzma$(LZMA).tar.bz2 -P $(DOWNLOAD)

$(DOWNLOAD)/ccache-$(CCACHE):
	test -f $(DOWNLOAD)/ccache-$(CCACHE).tar.gz || \
	wget http://samba.org/ftp/ccache/ccache-$(CCACHE).tar.gz -P $(DOWNLOAD)


$(DOWNLOAD)/mtd_20050122:
	test -f $(DOWNLOAD)/mtd_20050122.orig.tar.gz || \
	wget http://ftp.debian.org/debian/pool/main/m/mtd/mtd_20050122.orig.tar.gz -P $(DOWNLOAD)

##########################################################################################
# imagebuilding

image:
	@echo -e "\033[1;33mbuild image for @device@\033[0m ..."
if DEBUG
	@echo DEBUG enabeld
else
	@echo DEBUG disabeld
endif

##########################################################################################
# cleaning

coreclean: kernel-header uclibc-clean binutils-clean coregcc-clean gdb-clean
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

