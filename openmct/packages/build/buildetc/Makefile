# Buildetc

INSTALL=	if [ ! -d $(NEWROOTDIR)/etc ]; then \
			$(CP) -a $(SRCDIR)/etc $(NEWROOTDIR)/etc ; \
		fi && \
		if [ ! -d $(NEWVARDIR) ]; then \
			$(CP) -a $(SRCDIR)/var/etc $(NEWVARDIR); \
		fi

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi && \
		if [ -d $(NEWVARDIR) ]; then \
			$(RM) -rf $(NEWVARDIR); \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
