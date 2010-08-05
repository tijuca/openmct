# Busybox (Basic Shell Utils)

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://busybox.net/downloads/busybox-1.8.0.tar.bz2

MAKEARGS=	CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) ARCH=mipsel

INSTALL= 	cd $(SRCDIR) && make CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) ARCH=mipsel \
		CONFIG_PREFIX=$(NEWROOTDIR) install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
