# torrent
DOWNLOAD=	http://libtorrent.rakshasa.no/downloads/libtorrent-0.8.3.tar.gz

CONFIGUREENV=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc \
		CXX=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-g++ \
		CFLAGS="-Os -I$(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0 -I$(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0/sigc++" \
		CXXFLAGS="-Os -I$(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0 -I$(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0/sigc++"

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --enable-shared --disable-aligned

INSTALL=	$(CP) $(SRCDIR)/src/.libs/libtorrent.so.6.0.2 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libtorrent.so.6.0.2 && \
		$(LN) -sf libtorrent.so.6.0.2 $(NEWROOTDIR)/lib/libtorrent.so.6 && \
		$(LN) -sf libtorrent.so.6.0.2 $(NEWROOTDIR)/lib/libtorrent.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
