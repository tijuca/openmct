# WebAdmin Interface
FILENAME=	www

BUILD=		true

INSTALL=	if [ ! -d $(NEWROOTDIR)/www ]; then \
			$(MKDIR) $(NEWROOTDIR)/www ; \
		fi ; \
		$(CP) -a $(SRCDIR)/* $(NEWROOTDIR)/www ; \
		$(REMOVESVN) $(NEWROOTDIR)/www

CLEAN=		true

include		$(OPENMCT_DEVEL_PREFIX)Rules
