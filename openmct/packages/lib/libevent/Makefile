# libevent
DOWNLOAD=	http://www.monkey.org/~provos/libevent-1.1b.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared

INSTALL=        $(CP) $(SRCDIR)/.libs/libevent-1.1b.so.1.0.2 $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/libevent-1.1b.so.1.0.2 && \
                $(LN) -sf libevent-1.1b.so.1.0.2 $(NEWROOTDIR)/lib/libevent.so && \
                $(LN) -sf libevent-1.1b.so.1.0.2 $(NEWROOTDIR)/lib/libevent-1.1b.so.1 && \
                cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
