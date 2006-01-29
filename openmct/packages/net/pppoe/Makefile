# PPPOE
DOWNLOAD=	http://www.roaringpenguin.com/penguin/pppoe/rp-pppoe-3.7.tar.gz

SRCDIRSUFFIX=	/src

CONFIGUREARGS=	CFLAGS="-Wall -Os" CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc \
		RANLIB=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-ranlib

INSTALL=	$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/{pppoe,pppoe-relay,pppoe-server,pppoe-sniff} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{pppoe,pppoe-relay,pppoe-server,pppoe-sniff}

include 	$(OPENMCT_DEVEL_PREFIX)Rules
