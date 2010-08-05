# PPTP

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/pptpclient/pptp-1.7.1.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc OPTIMIZE="-Os" PPPD="/sbin/pppd"

INSTALL=	$(CP) $(SRCDIR)/pptp $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/pptp

include		$(OPENMCT_DEVEL_PREFIX)Rules
