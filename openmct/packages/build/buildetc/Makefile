# Buildetc

INSTALL=	if [ ! -d $(NEWROOTDIR)/etc ]; then \
			$(CP) -a $(SRCDIR)/etc $(NEWROOTDIR)/etc ; \
		fi && \
		if [ ! -d $(NEWCONFDIR) ]; then \
			$(CP) -a $(SRCDIR)/conf $(NEWCONFDIR); \
		fi

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi && \
		if [ -d $(NEWCONFDIR) ]; then \
			$(RM) -rf $(NEWCONFDIR); \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
