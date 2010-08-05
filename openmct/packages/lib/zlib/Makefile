# zlib

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://zlib.net/fossils/zlib-1.2.3.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --shared

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os"

INSTALL=	$(CP) $(SRCDIR)/libz.so.1.2.3 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libz.so.1.2.3 && \
		$(LN) -sf libz.so.1.2.3 $(NEWROOTDIR)/lib/libz.so.1 && \
		$(LN) -sf libz.so.1.2.3 $(NEWROOTDIR)/lib/libz.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
