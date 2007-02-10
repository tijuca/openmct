# PPP
DOWNLOAD=	ftp://ftp.samba.org/pub/ppp/ppp-2.4.4.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc COPTS=-Os FILTER=

INSTALL=	$(CP) $(SRCDIR)/pppd/pppd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/pppd && \
		$(CP) $(SRCDIR)/chat/chat $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/chat && \
		$(CP) $(SRCDIR)/pppdump/pppdump $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/pppdump && \
		$(CP) $(SRCDIR)/pppstats/pppstats $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/pppstats

include		$(OPENMCT_DEVEL_PREFIX)Rules
