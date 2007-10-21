# Buildetc

# this file is deprecated!

INSTALL=	if [ ! -d $(NEWROOTDIR)/etc ]; then \
			$(CP) -a $(SRCDIR)/etc $(NEWROOTDIR)/etc ; \
		fi && \
		if [ ! -d $(NEWVARDIR) ]; then \
			$(MKDIR) -p $(NEWVARDIR)/etc ; \
			$(CP) -a $(SRCDIR)/var/etc $(NEWVARDIR) ; \
		fi && \
		$(CP) $(SRCDIR)/sbin/reset $(NEWROOTDIR)/sbin/reset && \
		$(CHMOD) 755 $(NEWROOTDIR)/sbin/reset

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi && \
		if [ -d $(NEWVARDIR) ]; then \
			$(RM) -rf $(NEWVARDIR); \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
