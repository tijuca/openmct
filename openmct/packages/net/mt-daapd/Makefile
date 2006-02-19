# mt-daapd
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/mt-daapd/mt-daapd-0.2.2.tar.gz

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --disable-mdns

INSTALL=	$(CP) $(SRCDIR)/src/mt-daapd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/mt-daapd

include		$(OPENMCT_DEVEL_PREFIX)Rules
