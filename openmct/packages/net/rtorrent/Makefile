# rTorrent
DOWNLOAD=	http://libtorrent.rakshasa.no/downloads/rtorrent-0.7.9.tar.gz

#DEPENDSON=	packages/lib/curl \
#		packages/lib/sigc \
#		packages/lib/torrent 

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ \
		RANLIB=$(OPENMCT_CROSS_COMPILE)ranlib \
		AR=$(OPENMCT_CROSS_COMPILE)ar \
		CFLAGS="-Os" \
		CXXFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/ncurses -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.0" \
		STUFF_LIBS="-ltorrent -lsigc-2.0 -lcurl" \
		PKG_CONFIG_PATH="$(OPENMCT_CROSS_BASE)/lib/pkgconfig" \
		PATH="$(OPENMCT_CROSS_BASE)/bin:$(PATH)"

INSTALL=	$(CP) $(SRCDIR)/src/rtorrent $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/rtorrent

include		$(OPENMCT_DEVEL_PREFIX)Rules
