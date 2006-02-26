# Buildroot
DOWNLOAD=	http://buildroot.uclibc.org/downloads/snapshots/buildroot-20051202.tar.bz2

MAKEARGS=	BUILD_DIR=$(OUTPUTDIR)/$(TARGET)

INSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(CP) -a $(ROOTDIR) $(NEWROOTDIR) ; \
			$(CLEANUPROOT) $(NEWROOTDIR) ; \
		fi

UNINSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(RM) -rf $(NEWROOTDIR) ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
