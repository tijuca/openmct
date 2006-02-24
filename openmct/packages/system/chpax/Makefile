# GRADM
DOWNLOAD=	http://pax.grsecurity.net/chpax-0.7.tar.gz

MAKEARGS=	LIBS="" CFLAGS="-Wall -Os" CC=$(OPENMCT_CROSS_COMPILE)gcc

INSTALL=	$(CP) $(SRCDIR)/chpax $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/chpax

include		$(OPENMCT_DEVEL_PREFIX)Rules
