# Buildroot
DOWNLOAD=	http://buildroot.uclibc.org/downloads/snapshots/buildroot-20051202.tar.bz2

MAKEARGS=	BUILD_DIR=$(OUTPUTDIR)

INSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(CP) -a $(ROOTDIR) $(NEWROOTDIR) ; \
			$(RM) -rf $(NEWROOTDIR)/home ; \
			$(RM) -rf $(NEWROOTDIR)/mnt ; \
			$(RM) -rf $(NEWROOTDIR)/opt ; \
			$(RM) -rf $(NEWROOTDIR)/root ; \
			$(RM) -rf $(NEWROOTDIR)/usr ; \
			$(RM) -rf $(NEWROOTDIR)/var ; \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
			$(RM) -f $(NEWROOTDIR)/lib/libstdc++* ; \
			$(MKDIR) $(NEWROOTDIR)/usr ; \
			$(MKDIR) $(NEWROOTDIR)/usr/home ; \
			$(MKDIR) $(NEWROOTDIR)/usr/var ; \
			$(MKDIR) $(NEWROOTDIR)/usr/vservers ; \
			$(LN) -s usr/home $(NEWROOTDIR)/home ; \
			$(LN) -s usr/var $(NEWROOTDIR)/var ; \
			$(LN) -s usr/vservers $(NEWROOTDIR)/vservers ; \
		fi

UNINSTALL=	if [ ! -d $(NEWROOTDIR) ]; then \
			$(RM) -rf $(NEWROOTDIR) ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
