# torrent
DOWNLOAD=	http://downloads.sourceforge.net/libtorrent/libtorrent-0.12.tar.gz
#http://libtorrent.rakshasa.no/downloads/libtorrent-0.11.8.tar.gz

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1/sigc++" \
		CXXFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1/sigc++"

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared --disable-aligned 
#\
#		--with-openssl=$(OPENMCT_CROSS_BASE)

INSTALL=	$(CP) $(SRCDIR)/src/.libs/libtorrent.so.9.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libtorrent.so.9.0.0 && \
		$(LN) -sf libtorrent.so.9.0.0 $(NEWROOTDIR)/lib/libtorrent.so.6 && \
		$(LN) -sf libtorrent.so.9.0.0 $(NEWROOTDIR)/lib/libtorrent.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
