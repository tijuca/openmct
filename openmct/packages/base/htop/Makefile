# htop

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/htop/htop-0.6.6.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE) -I$(OPENMCT_CROSS_INCLUDE)/ncurses"

INSTALL=	$(CP) $(SRCDIR)/htop $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/htop

include		$(OPENMCT_DEVEL_PREFIX)Rules
