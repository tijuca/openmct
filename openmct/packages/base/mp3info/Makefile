# mp3info
DOWNLOAD=	ftp://ftp.ibiblio.org/pub/linux/apps/sound/mp3-utils/mp3info/mp3info-0.8.5a.tgz

MAKEARGS=       CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os" mp3info

INSTALL=	$(CP) $(SRCDIR)/mp3info $(NEWROOTDIR)/sbin && \
		$(STRIP)  $(NEWROOTDIR)/sbin/mp3info

include 	$(OPENMCT_DEVEL_PREFIX)Rules
