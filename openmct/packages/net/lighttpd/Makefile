# lighttpd
DOWNLOAD=	http://www.lighttpd.net/download/lighttpd-1.4.18.tar.gz	

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CCOPT="-Os"

INSTALL=	$(CP) $(SRCDIR)/thttpd $(NEWROOTDIR)/sbin/lighttpd && \
		$(STRIP) $(NEWROOTDIR)/sbin/lighttpd

include		$(OPENMCT_DEVEL_PREFIX)Rules
