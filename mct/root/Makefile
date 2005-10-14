#
# MCT Firmware Builder
#
# Andi <andi@void.at>

# Target
TARGET=mipsel-linux

# Binaries
ACTDIR=$(shell /bin/pwd)
ECHO=/bin/echo
WGET=/usr/bin/wget -Nc --passive-ftp -P $(DOWNLOADDIR)
TAR=/bin/tar
CP=/bin/cp
PATCH=/usr/bin/patch
ZCAT=/bin/zcat
GZIP=/bin/gzip
MAKE=/usr/bin/make
TOUCH=/usr/bin/touch
GCC=/usr/bin/gcc

# Directories
DOWNLOADDIR=$(ACTDIR)/downloads
CONFIGDIR=$(ACTDIR)/configs
PATCHDIR=$(ACTDIR)/patches
PACKAGEDIR=$(ACTDIR)/packages
OUTPUTDIR=$(ACTDIR)/output
ROOTDIR=$(ACTDIR)/root

# Files
ROOTTARGZ=$(OUTPUTDIR)/root.tar.gz
IMAGEVER=0x0106
IMAGESUBVER=0x1
IMAGE=$(OUTPUTDIR)/mct-$(IMAGEVER)-$(IMAGESUBVER).bin

# MCT Info Files
CONFIG=.config
PATCHED=.patched
BUILD=.build

# Source
MCT_MODIFY=mct_modify

# Base Software Packages

# Buildroot (Cross Compiler, Libs, etc..)
BUILDROOTBASEURL=http://buildroot.uclibc.org/downloads/snapshots
BUILDROOTFILE=buildroot-snapshot.tar.bz2
BUILDROOT=$(shell basename $(BUILDROOTFILE) -snapshot.tar.bz2)

# Busybox (Basic Shell Utils)
BUSYBOXBASEURL=http://www.busybox.net/downloads/snapshots
BUSYBOXFILE=busybox-snapshot.tar.bz2
BUSYBOX=$(shell basename $(BUSYBOXFILE) -snapshot.tar.bz2)

SSTRIP=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-sstrip

all:	directories buildroot busybox prepareroot

directories:
	$(shell if [ ! -d $(DOWNLOADDIR) ]; then mkdir $(DOWNLOADDIR); fi)
	$(shell if [ ! -d $(PACKAGEDIR) ]; then mkdir $(PACKAGEDIR); fi)
	$(shell if [ ! -d $(OUTPUTDIR) ]; then mkdir $(OUTPUTDIR); fi)

buildroot:	
	@if [ ! -f $(DOWNLOADDIR)/$(BUILDROOTFILE) ]; then \
		$(ECHO) WGET $(BUILDROOT) ; \
		$(WGET) $(BUILDROOTBASEURL)/$(BUILDROOTFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(BUILDROOT) ]; then \
		$(ECHO) UNPACK $(BUILDROOT) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(BUILDROOTFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(BUILDROOT)/$(PATCHED) ]; then \
		$(ECHO) PATCH $(BUILDROOT) ; \
		$(PATCH) -d $(PACKAGEDIR) -p0 < $(PATCHDIR)/$(BUILDROOT)-mct.diff ; \
		$(TOUCH) $(PACKAGEDIR)/$(BUILDROOT)/$(PATCHED) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(BUILDROOT)/$(CONFIG) ]; then \
		$(ECHO) BUILD $(BUILDROOT) ; \
		$(CP) $(CONFIGDIR)/$(BUILDROOT)-mct.config $(PACKAGEDIR)/$(BUILDROOT)/$(CONFIG) ; \
		cd $(PACKAGEDIR)/$(BUILDROOT) && $(MAKE) oldconfig && $(MAKE) BUILD_DIR=$(ACTDIR) ; \
	fi
	@$(TAR) cfz $(ROOTTARGZ) root

busybox:	directories
	@if [ ! -f $(DOWNLOADDIR)/$(BUSYBOXFILE) ]; then \
		$(ECHO) WGET $(BUSYBOX) ; \
		$(WGET) $(BUSYBOXBASEURL)/$(BUSYBOXFILE) ; \
	fi
	@if [ ! -d $(PACKAGEDIR)/$(BUSYBOX) ]; then \
		$(ECHO) UNPACK $(BUSYBOX) ; \
		$(TAR) -xj -C $(PACKAGEDIR) -f $(DOWNLOADDIR)/$(BUSYBOXFILE) ; \
	fi
	@if [ ! -f $(PACKAGEDIR)/$(BUSYBOX)/$(CONFIG) ]; then \
		$(ECHO) BUILD $(BUSYBOX) ; \
		$(CP) $(CONFIGDIR)/$(BUSYBOX)-mct.config $(PACKAGEDIR)/$(BUSYBOX)/$(CONFIG) ; \
		cd $(PACKAGEDIR)/$(BUSYBOX) && $(MAKE) oldconfig && \
		$(MAKE) CROSS=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc- ;\
	fi

prepareroot:	busybox
	$(shell if [ -d $(ROOTDIR) ]; then rm -rf $(ROOTDIR); fi)
	@$(RM) $(OUTPUTDIR)/root.squashfs
	@$(TAR) xfz $(ROOTTARGZ)
	@$(RM) -rf $(ROOTDIR)/home
	@$(RM) -rf $(ROOTDIR)/mnt
	@$(RM) -rf $(ROOTDIR)/opt
	@$(RM) -rf $(ROOTDIR)/root
	@$(RM) -rf $(ROOTDIR)/usr
	@$(RM) -rf $(ROOTDIR)/etc
	@$(RM) -f $(ROOTDIR)/lib/libstdc++*
	@$(CP) -a $(ACTDIR)/etc $(ROOTDIR)

	@$(ECHO) INSTALL $(BUSYBOX)
	@$(CP) $(PACKAGEDIR)/$(BUSYBOX)/busybox $(ROOTDIR)/bin
	@$(SSTRIP) $(ROOTDIR)/bin/busybox

	@$(PACKAGEDIR)/$(SQUASHFS)/squashfs-tools/mksquashfs $(ROOTDIR)/* $(OUTPUTDIR)/root.squashfs -le -all-root
