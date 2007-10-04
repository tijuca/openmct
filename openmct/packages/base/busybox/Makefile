# Busybox (Basic Shell Utils)
DOWNLOAD=	http://busybox.net/downloads/busybox-1.7.2.tar.bz2

MAKEARGS=	CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE)

INSTALL= 	cd $(SRCDIR) && make CONFIG_PREFIX=$(NEWROOTDIR) install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
