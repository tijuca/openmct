# cTorrent
OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://downloads.sourceforge.net/ctorrent/ctorrent-1.3.4.tar.bz2

INSTALL=	$(CP) $(SRCDIR)/ctorrent $(NEWROOTDIR)/bin && \
            $(STRIP) $(NEWROOTDIR)/bin/ctorrent

include		$(OPENMCT_DEVEL_PREFIX)Rules
