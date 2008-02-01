# Buildetc
#INSTALL=	$(RM) -rf $(NEWROOTDIR)/var/etc ; \
#		$(SVN) export --force $(SRCDIR)/etc $(NEWROOTDIR)/etc; \
		$(SVN) export --force $(SRCDIR)/new $(NEWROOTDIR)

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi

CLEAN=		true

include		$(OPENMCT_DEVEL_PREFIX)Rules
