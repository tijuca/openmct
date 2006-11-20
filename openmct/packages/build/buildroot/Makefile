# Buildroot
DOWNLOAD=	http://www.openmct.org/pub/build/buildroot-20051202.tar.bz2
#DOWNLOAD=	http://claxan.hangar18.org/misc/openmct/buildroot-20051202.tar.bz2

MAKEARGS=	BUILD_DIR=$(OUTPUTDIR)

INSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(CP) -a $(ROOTDIR) $(NEWROOTDIR) ; \
			$(CLEANUPROOT) $(NEWROOTDIR) ; \
			$(LN) -s ../etc/var $(NEWROOTDIR)/var ; \
		fi

UNINSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(RM) -rf $(NEWROOTDIR) ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
