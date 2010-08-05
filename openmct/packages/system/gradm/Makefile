# GRADM

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.grsecurity.net/gradm-2.1.10-200701121959.tar.gz

MAKEARGS=	LIBS="" CFLAGS="-Wall -Os" CC=$(OPENMCT_CROSS_COMPILE)gcc nopam

INSTALL=	$(CP) $(SRCDIR)/{gradm,grlearn} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{gradm,grlearn}

include		$(OPENMCT_DEVEL_PREFIX)Rules
