# GD
DOWNLOAD=	http://www.boutell.com/gd/http/gd-2.0.33.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET)

INSTALL=        $(CP) $(SRCDIR)/.libs/libgd.so.2.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libgd.so.2.0.0 && \
		$(LN) -sf libgd.so.2.0.0 $(NEWROOTDIR)/lib/libgd.so.2 && \
		$(LN) -sf libgd.so.2.0.0 $(NEWROOTDIR)/lib/libgd.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
