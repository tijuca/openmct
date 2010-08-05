# mtd-utils

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.infradead.org/pub/mtd-utils/mtd-utils-1.0.0.tar.gz

DEPENDSON=	packages/lib/zlib

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I./include -I$(OPENMCT_CROSS_INCLUDE)"

INSTALL=	$(CP) $(SRCDIR)/flashcp $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/flashcp && \
		$(CP) $(SRCDIR)/flash_erase $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/flash_erase

include		$(OPENMCT_DEVEL_PREFIX)Rules
