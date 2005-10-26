#
# MCT Firmware Builder
#
# Andi <andi@void.at>

include $(TOP)/Rules

# Files
ROOTTARGZ=$(OUTPUTDIR)/root.tar.gz

# Buildroot (Cross Compiler, Libs, etc..)
BUILDROOTBASEURL=http://buildroot.uclibc.org/downloads/snapshots
BUILDROOTFILE=buildroot-snapshot.tar.bz2
BUILDROOT=$(shell basename $(BUILDROOTFILE) -snapshot.tar.bz2)

# Busybox (Basic Shell Utils)
BUSYBOXBASEURL=http://www.busybox.net/downloads/snapshots
BUSYBOXFILE=busybox-snapshot.tar.bz2
BUSYBOX=$(shell basename $(BUSYBOXFILE) -snapshot.tar.bz2)

SSTRIP=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-sstrip

all:	directories buildroot busybox mct_flash prepareroot

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

busybox:	
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

mct_flash:
	@if [ ! -f $(SRCDIR)/$(BUILD) ]; then \
		$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc $(SRCDIR)/$(MCT_FLASH).c -o $(SRCDIR)/$(MCT_FLASH) ; \
		$(TOUCH) $(SRCDIR)/$(BUILD) ; \
	fi

prepareroot:	
	$(shell if [ -d $(ROOTDIR) ]; then rm -rf $(ROOTDIR); fi)
	@$(RM) $(OUTPUTDIR)/$(ROOTSQUASHFS)
	@$(TAR) xfz $(ROOTTARGZ)
	@$(CP) -a $(ROOTDIR) $(NEWROOTDIR)
	@$(RM) -rf $(NEWROOTDIR)/home
	@$(RM) -rf $(NEWROOTDIR)/mnt
	@$(RM) -rf $(NEWROOTDIR)/opt
	@$(RM) -rf $(NEWROOTDIR)/root
	@$(RM) -rf $(NEWROOTDIR)/usr
	@$(RM) -rf $(NEWROOTDIR)/etc
	@$(RM) -f $(NEWROOTDIR)/lib/libstdc++*
	@$(CP) -a $(ACTDIR)/etc $(NEWROOTDIR)
	@$(CP) -a $(SRCDIR)/$(MCT_FLASH) $(NEWROOTDIR)/sbin

	@$(ECHO) INSTALL $(BUSYBOX)
	@$(CP) $(PACKAGEDIR)/$(BUSYBOX)/busybox $(NEWROOTDIR)/sbin
	@$(SSTRIP) $(NEWROOTDIR)/sbin/busybox

	@$(ECHO) MAKE SYMLINKS FOR $(BUSYBOX)
	$(foreach link, $(shell cat busybox.mct.links), $(shell ln -s --force /sbin/busybox $(NEWROOTDIR)$(link)))

	@$(ECHO) INSTALL $(MCT_FLASH)
	@$(CP) $(SRCDIR)/$(MCT_FLASH) $(NEWROOTDIR)/sbin
	@$(SSTRIP) $(NEWROOTDIR)/sbin/$(MCT_FLASH)

	@$(MKSQUASHFS) $(NEWROOTDIR)/* $(OUTPUTDIR)/$(ROOTSQUASHFS) $(SQUASHFSOPTS)

clean:
	$(shell if [ -d $(PACKAGEDIR) ]; then rm -rf $(PACKAGEDIR); fi)
	$(shell if [ -d $(OUTPUTDIR) ]; then rm -rf $(OUTPUTDIR); fi)
	$(shell if [ -d $(ROOTDIR) ]; then rm -rf $(ROOTDIR); fi)
	$(shell if [ -d $(NEWROOTDIR) ]; then rm -rf $(NEWROOTDIR); fi)

distclean:      clean
	$(shell if [ -d $(DOWNLOADDIR) ]; then rm -rf $(DOWNLOADDIR); fi)

