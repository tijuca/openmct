#
# Kernel Builder
#
# Andi <andi@void.at>

include $(TOP)Rules

PATH=$(TARGETDIR)/bin:/usr/local/bin:/usr/bin:/bin

# Kernel
KERNELBASEURL=http://www.de.kernel.org/pub/linux/kernel/v2.4
KERNELFILE=linux-2.4.31.tar.bz2
KERNEL=$(shell basename $(KERNELFILE) .tar.bz2)

# SquashFS 
SQUASHFSBASEURL=http://heanet.dl.sourceforge.net/sourceforge/squashfs
SQUASHFSFILE=squashfs2.2-r2.tar.gz
SQUASHFS=$(shell basename $(SQUASHFSFILE) .tar.gz)

# Binutils
BINUTILSBASEURL=ftp://ftp.gnu.org/gnu/binutils
BINUTILSFILE=binutils-2.15.tar.bz2
BINUTILS=$(shell basename $(BINUTILSFILE) .tar.bz2)

# GCC Core
GCCCOREBASEURL=ftp://ftp.gnu.org/gnu/gcc/gcc-3.3.4
GCCCOREFILE=gcc-core-3.3.4.tar.bz2
GCCCORE=gcc-3.3.4

# GLIBC
GLIBCBASEURL=ftp://ftp.gnu.org/gnu/glibc
GLIBCFILE=glibc-2.2.5.tar.gz
GLIBC=glibc-2.2.5

# GLIBC Threads
GLIBCTHREADSBASEURL=ftp://ftp.gnu.org/gnu/glibc
GLIBCTHREADSFILE=glibc-linuxthreads-2.2.5.tar.gz
GLIBCTHREADS=linuxthreads

# EGCS
EGCSBASEURL=ftp://ftp.fu-berlin.de/unix/languages/gcc/old-releases/egcs
EGCSFILE=egcs-1.1.2.tar.bz2
EGCS=egcs-1.1.2

all:	directories squashfs preparekernel binutils gcccore glibc gcc buildkernel

directories:
	$(shell if [ ! -d $(DOWNLOADDIR) ]; then mkdir $(DOWNLOADDIR); fi)
	$(shell if [ ! -d $(PACKAGEDIR) ]; then mkdir $(PACKAGEDIR); fi)
	$(shell if [ ! -d $(OUTPUTDIR) ]; then mkdir $(OUTPUTDIR); fi)

squashfs:	
	@if [ ! -f $(DOWNLOADDIR)/$(SQUASHFSFILE) ]; then \
		$(ECHO) WGET $(SQUASHFS) ; \
		$(WGET) $(SQUASHFSBASEURL)/$(SQUASHFSFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(SQUASHFS) ]; then \
		$(ECHO) UNPACK $(SQUASHFS) ; \
		$(TAR) -xz -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(SQUASHFSFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(SQUASHFS)/$(CONFIG) ]; then \
		$(ECHO) BUILD $(SQUASHFS) ; \
		cd $(PACKAGEDIR)/$(SQUASHFS)/squashfs-tools && \
		$(MAKE) ; \
	fi

preparekernel:
	@if [ ! -f $(DOWNLOADDIR)/$(KERNELFILE) ]; then \
		$(ECHO) WGET $(KERNEL) ; \
		$(WGET) $(KERNELBASEURL)/$(KERNELFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(KERNEL) ]; then \
		$(ECHO) UNPACK $(KERNEL) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(KERNELFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(KERNEL)/$(PATCHED) ]; then \
		$(ECHO) PATCH $(KERNEL) : MCT ; \
		$(PATCH) -d $(PACKAGEDIR) -p0 < $(PATCHDIR)/$(KERNEL)-mct.diff ; \
		$(PATCH) -d $(PACKAGEDIR) -p0 < $(PACKAGEDIR)/$(SQUASHFS)/$(KERNEL)/squashfs2.2-patch ; \
		$(TOUCH) $(PACKAGEDIR)/$(KERNEL)/$(PATCHED) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(KERNEL)/$(CONFIG) ]; then \
		$(ECHO) CONFIGURE $(KERNEL) ; \
		$(CP) $(CONFIGDIR)/$(KERNEL)-mct.config $(PACKAGEDIR)/$(KERNEL)/.config ; \
		cd $(PACKAGEDIR)/$(KERNEL) && $(MAKE) oldconfig dep ; \
		$(LN) -s $(PACKAGEDIR)/$(KERNEL) $(ACTDIR)/linux ; \
	fi

binutils:	
	@if [ ! -f $(DOWNLOADDIR)/$(BINUTILSFILE) ]; then \
		$(ECHO) WGET $(BINUTILS) ; \
		$(WGET) $(BINUTILSBASEURL)/$(BINUTILSFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(BINUTILS) ]; then \
		$(ECHO) UNPACK $(BINUTILS) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(BINUTILSFILE) ; \
        fi
	@if [ ! -f $(PACKAGEDIR)/$(BINUTILS)/$(CONFIG) ]; then \
		$(ECHO) CONFIGURE $(BINUTILS) ; \
		cd $(PACKAGEDIR)/$(BINUTILS) && \
		./configure --target=$(TARGET) --prefix=$(TARGETDIR) && \
		$(MAKE) all install && \
		touch $(PACKAGEDIR)/$(BINUTILS)/$(CONFIG) ; \
        fi

gcccore:	
	@if [ ! -f $(DOWNLOADDIR)/$(GCCCOREFILE) ]; then \
		$(ECHO) WGET $(GCCCORE) ; \
		$(WGET) $(GCCCOREBASEURL)/$(GCCCOREFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(GCCCORE) ]; then \
		$(ECHO) UNPACK $(GCCCORE) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(GCCCOREFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(GCCCORE)/$(CONFIG) ]; then \
		$(ECHO) CONFIGURE $(GCCCORE) ; \
		cd $(PACKAGEDIR)/$(GCCCORE) && \
		./configure --target=$(TARGET) --prefix=$(TARGETDIR) \
			--enable-languages="c" --with-gnu-as --with-gnu-ld --disable-threads \
			--disable-shared && \
		$(MAKE) all-gcc install-gcc && \
		touch $(PACKAGEDIR)/$(GCCCORE)/$(CONFIG) ; \
        fi

glibc:	
	@if [ ! -f $(DOWNLOADDIR)/$(GLIBCFILE) ]; then \
		$(ECHO) WGET $(GLIBC) ; \
		$(WGET) $(GLIBCBASEURL)/$(GLIBCFILE) ; \
	fi
	@if [ ! -f $(DOWNLOADDIR)/$(GLIBCTHREADSFILE) ]; then \
		$(ECHO) WGET $(GLIBCTHREADS) ; \
		$(WGET) $(GLIBCTHREADSBASEURL)/$(GLIBCTHREADSFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(GLIBC) ]; then \
		$(ECHO) UNPACK $(GLIBC) ; \
		$(TAR) -xz -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(GLIBCFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(GLIBC)/$(GLIBCTHREADS) ]; then \
		$(ECHO) UNPACK $(GLIBCTHREADS) ; \
		$(TAR) -xz -C $(PACKAGEDIR)/$(GLIBC) -f $(DOWNLOADDIR)/$(GLIBCTHREADSFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(GLIBC)/$(PATCHED) ]; then \
		$(ECHO) PATCH $(GLIBC) ; \
		$(PATCH) -d $(PACKAGEDIR) -p0 < $(PATCHDIR)/$(GLIBC)-mct.diff ; \
		$(TOUCH) $(PACKAGEDIR)/$(GLIBC)/$(PATCHED) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(GLIBC)/$(CONFIG) ]; then \
		$(ECHO) CONFIGURE $(GLIBC) ; \
		cd $(PACKAGEDIR)/$(GLIBC) && \
		CC=$(TARGET)-gcc AR=$(TARGET)-ar RANLIB=$(TARGET)-ranlib \
		./configure --host=$(TARGET) --with-headers=$(PACKAGEDIR)/$(KERNEL)/include \
		--enable-add-ons --prefix=$(TARGETDIR)/$(TARGET) && \
		$(MAKE) all install && \
		$(TOUCH) $(PACKAGEDIR)/$(GLIBC)/$(CONFIG) ; \
	fi

gcc:	
	@if [ ! -f $(DOWNLOADDIR)/$(EGCSFILE) ]; then \
		$(ECHO) WGET $(EGCS) ; \
		$(WGET) $(EGCSBASEURL)/$(EGCSFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(EGCS) ]; then \
		$(ECHO) UNPACK $(EGCS) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(EGCSFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(EGCS)/$(PATCHED) ]; then \
		$(ECHO) PATCH $(EGCS) ; \
		$(PATCH) -d $(PACKAGEDIR) -p0 < $(PATCHDIR)/$(EGCS)-mct.diff ; \
		$(TOUCH) $(PACKAGEDIR)/$(EGCS)/$(PATCHED) ; \
        fi
	@if [ ! -f $(PACKAGEDIR)/$(EGCS)/$(CONFIG) ]; then \
		$(ECHO) CONFIGURE $(EGCS) ; \
		cd $(PACKAGEDIR)/$(EGCS) && \
		./configure --target=$(TARGET) --prefix=$(TARGETDIR) \
			--enable-languages="c" --with-gnu-as --with-gnu-ld --disable-threads \
			--disable-shared --disable-multilib && \
		$(MAKE) all-gcc install-gcc && \
		touch $(PACKAGEDIR)/$(EGCS)/$(CONFIG) ; \
        fi

buildkernel:
	@if [ ! -f $(PACKAGEDIR)/$(KERNEL)/$(BUILD) ]; then \
		$(ECHO) BUILD $(KERNEL) ; \
		cd $(PACKAGEDIR)/$(KERNEL) && $(MAKE) CROSS_COMPILE=$(TARGET)- vmlinux && \
		$(CP) $(PACKAGEDIR)/$(KERNEL)/vmlinux.bin $(OUTPUTDIR) && \
		$(GZIP) -9 $(OUTPUTDIR)/vmlinux.bin && \
		$(TOUCH) $(PACKAGEDIR)/$(KERNEL)/$(BUILD) ; \
        fi
	
clean:
	$(shell if [ -d $(PACKAGEDIR) ]; then rm -rf $(PACKAGEDIR); fi)
	$(shell if [ -d $(OUTPUTDIR) ]; then rm -rf $(OUTPUTDIR); fi)
	$(shell if [ -h linux ]; then rm linux; fi )

distclean:	clean
	$(shell if [ -d $(DOWNLOADDIR) ]; then rm -rf $(DOWNLOADDIR); fi)
