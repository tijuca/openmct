# Busybox (Basic Shell Utils)
DOWNLOAD=	http://busybox.net/downloads/busybox-1.1.2.tar.bz2

MAKEARGS=	CROSS=$(OPENMCT_CROSS_COMPILE)

INSTALL= 	cd $(SRCDIR) && make PREFIX=$(NEWROOTDIR) install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
