# Buildroot
DOWNLOAD=	http://buildroot.uclibc.org/downloads/buildroot-snapshot.tar.bz2

MAKEARGS=	BUILD_DIR=$(OUTPUTDIR)

INSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(CP) -a $(ROOTDIR) $(NEWROOTDIR) ; \
			$(CLEANUPROOT) $(NEWROOTDIR) ; \
		fi

UNINSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(RM) -rf $(NEWROOTDIR) ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
