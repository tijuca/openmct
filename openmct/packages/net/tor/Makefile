# TOR

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://tor.eff.org/dist/tor-0.1.2.17.tar.gz

DEPENDSON=	packages/lib/zlib

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) 

INSTALL=	$(CP) $(SRCDIR)/src/or/tor $(NEWROOTDIR)/sbin/tor && \
		$(STRIP) $(NEWROOTDIR)/sbin/tor

include		$(OPENMCT_DEVEL_PREFIX)Rules
