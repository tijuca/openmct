# nano
DOWNLOAD=	http://www.nano-editor.org/dist/v2.0/nano-2.0.3.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE) -I$(OPENMCT_CROSS_INCLUDE)/ncurses"

INSTALL=	$(CP) $(SRCDIR)/nano $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/nano

include		$(OPENMCT_DEVEL_PREFIX)Rules
