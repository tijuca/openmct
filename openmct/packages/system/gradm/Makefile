# GRADM
DOWNLOAD=	http://www.grsecurity.net/gradm-2.1.7-200511041858.tar.gz

MAKEARGS=	LIBS="" CFLAGS="-Wall -Os" CC=$(OPENMCT_CROSS_COMPILE)gcc nopam

INSTALL=	$(CP) $(SRCDIR)/{gradm,grlearn} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{gradm,grlearn}

include		$(OPENMCT_DEVEL_PREFIX)Rules
