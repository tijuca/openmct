# Buildetc

# this file is deprecated!

INSTALL=	$(SVN) export --force $(SRCDIR)/etc $(NEWROOTDIR)/etc && \
		$(SVN) export --force $(SRCDIR)/sbin $(NEWROOTDIR)/sbin

UNINSTALL=	if [ -d $(NEWROOTDIR)/etc ]; then \
			$(RM) -rf $(NEWROOTDIR)/etc ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
