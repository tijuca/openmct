# nano

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.nano-editor.org/dist/v2.0/nano-2.0.6.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE) -I$(OPENMCT_CROSS_INCLUDE)/ncurses"

INSTALL=	$(CP) $(SRCDIR)/src/nano $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/nano

include		$(OPENMCT_DEVEL_PREFIX)Rules
