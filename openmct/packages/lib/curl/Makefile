# curl

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://curl.haxx.se/download/curl-7.17.0.tar.bz2

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared --with-random=/dev/urandom

INSTALL=	$(CP) $(SRCDIR)/lib/.libs/libcurl.so.4.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libcurl.so.4.0.0 && \
		$(LN) -sf libcurl.so.4.0.0 $(NEWROOTDIR)/lib/libcurl.so.3 && \
		$(LN) -sf libcurl.so.4.0.0 $(NEWROOTDIR)/lib/libcurl.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
