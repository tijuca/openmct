# claxan toolchain Makefile
#
# Author: Andi <andi@void.at>
#
#   make PREFIX=~/mct
#
# THIS FILE IS FOR STUDYING PURPOSES ONLY AND A PROOF-OF-
# CONCEPT. THE AUTHOR CAN NOT BE HELD RESPONSIBLE FOR ANY
# DAMAGE DONE USING THIS PROGRAM.
#
DOWNLOADDIR=dl
BUILDDIR=build
PATCHDIR=patches
WGET=wget -Nc -P $(DOWNLOADDIR)
TAR=tar -C $(BUILDDIR)
MV=mv
TOUCH=touch
MKDIR=mkdir
BZCAT=bzcat
CP=cp
PATCH=patch -d $(BUILDDIR) -p0
TARGET=mipsel-linux
BASEURL=http://www.openmct.org/pub/original/toolchain/
ifndef PREFIX
PREFIX=/usr/local/$(TARGET)
endif
ifndef ARCH
ARCH=sa100
endif

BUILD_BINUTILS=.build_binutils
BUILD_GCCCORE=.build_gcccore
BUILD_LINUX=.build_linux
BUILD_GLIBC=.build_glibc
BUILD_GCC=.build_gcc
BUILD_UCLIBC=.build_uclibc

all:		$(BUILD_BINUTILS) $(BUILD_GCCCORE) $(BUILD_LINUX) $(BUILD_GLIBC) $(BUILD_GCC) $(BUILD_UCLIBC)

# Binutils
$(DOWNLOADDIR)/binutils-2.15.tar.bz2:
		@$(WGET) $(BASEURL)binutils-2.15.tar.bz2

$(BUILDDIR)/binutils-2.15: $(DOWNLOADDIR)/binutils-2.15.tar.bz2
		@$(TAR) -xjf $(DOWNLOADDIR)/binutils-2.15.tar.bz2

$(BUILD_BINUTILS): $(BUILDDIR)/binutils-2.15
		@cd $(BUILDDIR)/binutils-2.15 && \
		./configure --target=$(TARGET) --prefix=$(PREFIX) && \
		make all install && cd ../.. && $(TOUCH) $(BUILD_BINUTILS)

# GCC Core 3.3.4
$(DOWNLOADDIR)/gcc-core-3.3.4.tar.gz:
		@$(WGET) $(BASEURL)gcc-core-3.3.4.tar.gz

$(BUILDDIR)/gcc-core-3.3.4: $(DOWNLOADDIR)/gcc-core-3.3.4.tar.gz
		@$(TAR) -xzf $(DOWNLOADDIR)/gcc-core-3.3.4.tar.gz
		@$(MV) $(BUILDDIR)/gcc-3.3.4 $(BUILDDIR)/gcc-core-3.3.4

$(BUILD_GCCCORE): $(BUILDDIR)/gcc-core-3.3.4
		@PATH=$(PREFIX)/bin:$(PATH) cd $(BUILDDIR)/gcc-core-3.3.4 && \
		./configure --target=$(TARGET) \
		--prefix=$(PREFIX) --enable-languages="c" --with-gnu-as \
		--with-gnu-ld --disable-threads --disable-shared && \
		make all-gcc install-gcc && cd ../.. && $(TOUCH) $(BUILD_GCCCORE)

# Linux 2.4.16
$(DOWNLOADDIR)/linux-$(ARCH).tar.bz2:
		@$(WGET) $(BASEURL)/linux-$(ARCH).tar.bz2

$(BUILDDIR)/linux: $(DOWNLOADDIR)/linux-$(ARCH).tar.bz2
		@$(TAR) -xjf $(DOWNLOADDIR)/linux-$(ARCH).tar.bz2

$(BUILD_LINUX):	$(BUILDDIR)/linux
		$(MKDIR) -p $(PREFIX)/$(TARGET)/include && \
		$(CP) -r $(BUILDDIR)/linux/include/linux $(PREFIX)/$(TARGET)/include && \
		$(CP) -r $(BUILDDIR)/linux/include/asm-mips $(PREFIX)/$(TARGET)/include/asm && \
		$(TOUCH) $(BUILD_LINUX)

# Glibc 2.2.25
$(DOWNLOADDIR)/glibc-2.2.5.tar.gz:
		@$(WGET) $(BASEURL)glibc-2.2.5.tar.gz
$(DOWNLOADDIR)/glibc-linuxthreads-2.2.5.tar.gz:
		@$(WGET) $(BASEURL)glibc-linuxthreads-2.2.5.tar.gz

$(BUILDDIR)/glibc-2.2.5: $(DOWNLOADDIR)/glibc-2.2.5.tar.gz $(DOWNLOADDIR)/glibc-linuxthreads-2.2.5.tar.gz
		@$(TAR) -xzf $(DOWNLOADDIR)/glibc-2.2.5.tar.gz
		@tar -C $(BUILDDIR)/glibc-2.2.5 -xzf $(DOWNLOADDIR)/glibc-linuxthreads-2.2.5.tar.gz
		$(PATCH) < $(PATCHDIR)/glibc-2.2.5-mipsel-linux.diff

$(BUILD_GLIBC): $(BUILDDIR)/glibc-2.2.5
		@PATH=$(PREFIX)/bin:$(PATH) cd $(BUILDDIR)/glibc-2.2.5 && \
		CC=$(TARGET)-gcc AR=$(TARGET)-ar RANLIB=$(TARGET)-ranlib \
		../glibc-2.2.5/configure --host=$(TARGET) --with-headers=$(PREFIX)/$(TARGET)/include \
		--enable-add-ons --prefix=$(PREFIX)/$(TARGET) && \
		make all install && \
		cd ../.. && $(TOUCH) $(BUILD_GLIBC)

# GCC 3.3.4
$(DOWNLOADDIR)/gcc-3.3.4.tar.bz2:
		@$(WGET) $(BASEURL)gcc-3.3.4.tar.bz2

$(BUILDDIR)/gcc-3.3.4: $(DOWNLOADDIR)/gcc-3.3.4.tar.bz2
		@$(TAR) -xjf $(DOWNLOADDIR)/gcc-3.3.4.tar.bz2

$(BUILD_GCC): $(BUILDDIR)/gcc-3.3.4
		@PATH=$(PREFIX)/bin:$(PATH) cd $(BUILDDIR)/gcc-3.3.4 && \
		./configure --enable-languages="c,c++" --target=$(TARGET) --prefix=$(PREFIX) && \
		make all install && \
		cd ../.. && $(TOUCH) $(BUILD_GCC)

# Uclibc 0.9.14
$(DOWNLOADDIR)/uClibc-0.9.14.tar.bz2:
		@$(WGET) $(BASEURL)uClibc-0.9.14.tar.bz2

$(BUILDDIR)/uClibc-0.9.14:
		@$(TAR) -xjf $(DOWNLOADDIR)/uClibc-0.9.14.tar.bz2
		@$(PATCH) < $(PATCHDIR)/uClibc-0.9.14-mipsel-linux.diff

$(BUILD_UCLIBC): $(BUILDDIR)/uClibc-0.9.14
		@PATH=$(PREFIX)/bin:$(PATH) cd $(BUILDDIR)/uClibc-0.9.14 && \
		make KERNEL_SOURCE=$(shell pwd)/$(BUILDDIR)/linux DEVEL_PREFIX=$(PREFIX)/$(TARGET)-uclibc SYSTEM_DEVEL_PREFIX=$(PREFIX)/$(TARGET)-uclibc all && \
		make DEVEL_PREFIX=$(PREFIX)/$(TARGET)-uclibc install && \
		cd ../.. && $(TOUCH) $(BUILD_UCLIBC)
