# torrent
DOWNLOAD=	http://libtorrent.rakshasa.no/downloads/libtorrent-0.8.3.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.0 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.0/sigc++" \
		CXXFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.0 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.0/sigc++"

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared --disable-aligned

INSTALL=	$(CP) $(SRCDIR)/src/.libs/libtorrent.so.6.0.2 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libtorrent.so.6.0.2 && \
		$(LN) -sf libtorrent.so.6.0.2 $(NEWROOTDIR)/lib/libtorrent.so.6 && \
		$(LN) -sf libtorrent.so.6.0.2 $(NEWROOTDIR)/lib/libtorrent.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
