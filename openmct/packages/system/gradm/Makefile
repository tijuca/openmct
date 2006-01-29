# GRADM
DOWNLOAD=	http://www.grsecurity.net/gradm-2.1.7-200511041858.tar.gz

MAKEARGS=	LIBS="" CFLAGS="-Wall -Os" CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc nopam

INSTALL=	$(CP) $(SRCDIR)/{gradm,grlearn} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{gradm,grlearn}

include		$(OPENMCT_DEVEL_PREFIX)Rules
