# curl
DOWNLOAD=	http://curl.haxx.se/download/curl-7.15.1.tar.bz2

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --enable-shared --with-random=/dev/urandom

INSTALL=	$(CP) $(SRCDIR)/lib/.libs/libcurl.so.3.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libcurl.so.3.0.0 && \
		$(LN) -sf libcurl.so.3.0.0 $(NEWROOTDIR)/lib/libcurl.so.3 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
