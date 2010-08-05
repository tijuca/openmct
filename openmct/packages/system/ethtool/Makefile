# ethtool

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://downloads.sourceforge.net/project/gkernel/ethtool/6/ethtool-6.tar.gz

INSTALL=	$(CP) $(SRCDIR)/ethtool $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ethtool

include		$(OPENMCT_DEVEL_PREFIX)Rules
