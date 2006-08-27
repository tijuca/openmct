# TOR
DOWNLOAD=	http://tor.eff.org/dist/tor-0.1.1.23.tar.gz	

DEPENDSON=	packages/lib/zlib

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) 

INSTALL=	$(CP) $(SRCDIR)/src/or/tor $(NEWROOTDIR)/sbin/tor && \
		$(STRIP) $(NEWROOTDIR)/sbin/tor

include		$(OPENMCT_DEVEL_PREFIX)Rules
