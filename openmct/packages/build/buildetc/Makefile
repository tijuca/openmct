# Buildetc

INSTALL=	if [ ! -d $(NEWETCDIR) ]; then \
			$(CP) -a $(SRCDIR)/etc $(NEWETCDIR) ; \
		fi

UNINSTALL=	if [ -d $(NEWETCDIR) ]; then \
			$(RM) -rf $(NEWETCDIR) ; \
		fi

include		$(OPENMCT_DEVEL_PREFIX)Rules
