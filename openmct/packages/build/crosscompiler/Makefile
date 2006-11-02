# claxan toolchain Makefile
#
# Author: Andi <andi@void.at>
#
# + usage
#   make download
#
#   make extract
#
#   make patch
#
#   make kernel
#
#   make binutils
#
#   make coregcc
#
#   make glibc
#
#   make gcc
#
#   make uclibc
#
# THIS FILE IS FOR STUDYING PURPOSES ONLY AND A PROOF-OF-
# CONCEPT. THE AUTHOR CAN NOT BE HELD RESPONSIBLE FOR ANY
# DAMAGE DONE USING THIS PROGRAM.
#
WGET=wget -Nc --passive-ftp
BINUTILS=binutils-2.17.tar.bz2
BINUTILSURL=ftp://ftp.gnu.org/gnu/binutils/$(BINUTILS)
BINUTILSDIR=binutils-2.17
GCC=gcc-3.3.6.tar.bz2
GCCURL=ftp://ftp.gnu.org/gnu/gcc/gcc-3.3.6/$(GCC)
UCLIBC=uClibc-0.9.28.tar.bz2
UCLIBCURL=http://www.uclibc.org/downloads/$(UCLIBC)
PREFIX=$(shell pwd)/output
TARGET=mipsel-linux
BUILD=$(shell pwd)/build

all:		download $(PREFIX) $(BUILD)

download:	$(BINUTILS) $(GCC) $(UCLIBC)

$(BINUTILS):	
		$(WGET) $(BINUTILSURL)

$(GCC):
		$(WGET) $(GCCURL)

$(UCLIBC):
		$(WGET) $(UCLIBCURL)

$(PREFIX):
		@mkdir $(PREFIX)

$(BUILD):
		@mkdir $(BUILD)

$(BINUTILSDIR):
		@tar xfj $(BINUTILS)

binutils: $(BINUTILSDIR)
		@cd $(BINUTILSDIR) && \
		./configure --target=$(TARGET) --prefix=$(PREFIX) && \
		make all install
		
patch:		$(BUILDDIR)
		@echo "[*] patch source tree ... start"
		@cd $(BUILDDIR) && zcat ../$(DOWNLOADDIR)/egcs-1.1.2.diff.gz | $(PATCH)
		@cd $(BUILDDIR) && zcat ../$(DOWNLOADDIR)/glibc-2.2.5-mipsel-linux.diff.gz | $(PATCH)
		@cd $(BUILDDIR) && zcat ../$(DOWNLOADDIR)/uClibc-0.9.14-mipsel-linux.diff.gz | $(PATCH)
		@cd $(BUILDDIR) && zcat ../$(DOWNLOADDIR)/linux-2.4.31-mct.diff.gz | $(PATCH)
		@echo "[*] patch source tree ... stop"

kernel:
		@echo "[*] configure kernel ... start"
		@cd $(BUILDDIR)/linux && make CROSS_COMPILE=mipsel-linux clean
		@cp $(DOWNLOADDIR)/linux-claxan-2.4.16.config $(BUILDDIR)/linux/.config
		@cd $(BUILDDIR)/linux && make CROSS_COMPILE=mipsel-linux oldconfig dep
		@echo "[*] configure kernel ... stop"

coregcc:
		@echo "[*] compile and install core gcc... start"
		@cd $(BUILDDIR)/gcc-3.3.4 && \
		./configure --target=$(TARGET) --prefix=$(PREFIX) \
			--enable-languages="c" --with-gnu-as --with-gnu-ld --disable-threads \
			--disable-shared && \
		make all-gcc install-gcc
		@echo "[*] compile and install core gcc... stop"

glibc:
		@echo "[*] compile and install glibc ... start"
		cd $(BUILDDIR)/glibc-2.2.5 && \
		CC=$(TARGET)-gcc AR=$(TARGET)-ar RANLIB=$(TARGET)-ranlib \
		./configure --host=$(TARGET) --with-headers=../../$(BUILDDIR)/linux/include \
			--enable-add-ons --prefix=$(PREFIX)/$(TARGET) && \
		make all install
		@echo "[*] compile and install glibc ... stop"

gcc:
		@echo "[*] compile and install gcc ... start"
		cd $(BUILDDIR)/egcs-1.1.2 && \
		./configure --enable-languages="c,c++" --with-newlib --target=$(TARGET) --prefix=$(PREFIX) && \
		make SUBDIRS="libiberty texinfo gcc" ALL_TARGET_MODULES= \
		       	CONFIGURE_TARGET_MODULES= INSTALL_TARGET_MODULES= LANGUAGES="c" install && \
		make SUBDIRS="libiberty texinfo gcc" ALL_TARGET_MODULES= \
			CONFIGURE_TARGET_MODULES= INSTALL_TARGET_MODULES= LANGUAGES="c++" install
		@echo "[*] compile and install gcc ... stop"

uclibc:
		@echo "[*] compile and install uclibc ... start"
		cd $(BUILDDIR)/uClibc-0.9.14 && \
		make && make install
		@echo "[*] compile and install uclibc ... stop"

