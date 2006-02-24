# nano
DOWNLOAD=	http://www.nano-editor.org/dist/v1.2/nano-1.2.5.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/include  -I$(OPENMCT_CROSS_INCLUDE)/include/ncurses"

INSTALL=	$(CP) $(SRCDIR)/nano $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/nano

include		$(OPENMCT_DEVEL_PREFIX)Rules
