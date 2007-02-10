# PPPOE
DOWNLOAD=	http://www.roaringpenguin.com/penguin/pppoe/rp-pppoe-3.8.tar.gz

SRCDIRSUFFIX=	/src

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) \
		CFLAGS="-Wall -Os" CC=$(OPENMCT_CROSS_COMPILE)gcc \
		RANLIB=$(OPENMCT_CROSS_COMPILE)ranlib

INSTALL=	$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/{pppoe,pppoe-relay,pppoe-server,pppoe-sniff} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{pppoe,pppoe-relay,pppoe-server,pppoe-sniff}

include 	$(OPENMCT_DEVEL_PREFIX)Rules
