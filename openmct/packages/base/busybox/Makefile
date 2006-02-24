# Busybox (Basic Shell Utils)
DOWNLOAD=	http://www.busybox.net/downloads/snapshots/busybox-snapshot.tar.bz2

MAKEARGS=	CROSS=$(OPENMCT_CROSS_COMPILE)

INSTALL=	$(CP) $(SRCDIR)/busybox $(NEWROOTDIR)/sbin ; \
		$(STRIP) $(NEWROOTDIR)/sbin/busybox ; \
		$(MAKESYMLINKS) $(NEWROOTDIR) /sbin/busybox $(CONFIGDIR)/links

include		$(OPENMCT_DEVEL_PREFIX)Rules

all:
		@echo HALLO
