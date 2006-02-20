# rTorrent
DOWNLOAD=	http://libtorrent.rakshasa.no/downloads/rtorrent-0.4.3.tar.gz

DEPENDSON=	packages/lib/curl \
		packages/lib/sigc \
		packages/lib/torrent 

CONFIGUREENV=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc \
		CXX=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-g++ \
		RANLIB=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-ranlib \
		AR=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-ar \
		CFLAGS="-Os" \
		CXXFLAGS="-Os -I$(OUTPUTDIR)/$(TARGET)/include/ncurses -I$(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0" \
		STUFF_LIBS="-ltorrent -lsigc-2.0 -lcurl"

INSTALL=	$(CP) $(SRCDIR)/src/rtorrent $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/rtorrent

include		$(OPENMCT_DEVEL_PREFIX)Rules
