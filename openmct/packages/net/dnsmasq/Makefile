# dnsmasq
DOWNLOAD=	http://www.thekelleys.org.uk/dnsmasq/dnsmasq-2.40.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS=-Os

INSTALL=	$(CP) $(SRCDIR)/src/dnsmasq $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/dnsmasq

include		$(OPENMCT_DEVEL_PREFIX)Rules
