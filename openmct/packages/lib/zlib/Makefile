# zlib
DOWNLOAD=	http://www.zlib.net/zlib-1.2.3.tar.bz2

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --shared

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAGS="-Os"

INSTALL=	$(CP) $(SRCDIR)/libz.so.1.2.3 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libz.so.1.2.3 && \
		$(LN) -sf libz.so.1.2.3 $(NEWROOTDIR)/lib/libz.so.1 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
