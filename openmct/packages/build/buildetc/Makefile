# Buildetc
INSTALL=	$(SVN) export --force $(SRCDIR)/etc $(NEWROOTDIR)/etc

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
