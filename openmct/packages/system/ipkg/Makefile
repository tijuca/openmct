# ipkg

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.handhelds.org/download/packages/ipkg/ipkg-0.99.163.tar.gz

INSTALL=	$(CP) $(SRCDIR)/ipkg $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ipkg

include		$(OPENMCT_DEVEL_PREFIX)Rules
