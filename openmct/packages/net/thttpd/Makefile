# thttpd
DOWNLOAD=	http://www.acme.com/software/thttpd/thttpd-2.25b.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CCOPT="-Os"

INSTALL=	$(CP) $(SRCDIR)/thttpd $(NEWROOTDIR)/sbin/thttpd && \
		$(STRIP) $(NEWROOTDIR)/sbin/thttpd

include		$(OPENMCT_DEVEL_PREFIX)Rules
