# ggrab

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.menzebach.de/ggrab/ggrab-0.22a-linux.tgz

MAKEARGS=	CXX=$(OPENMCT_CROSS_COMPILE)g++ CC=$(OPENMCT_CROSS_COMPILE)gcc CCOPT="-Os"

INSTALL=	$(CP) $(SRCDIR)/ggrab $(NEWROOTDIR)/sbin/ggrab && \
		$(STRIP) $(NEWROOTDIR)/sbin/ggrab

include		$(OPENMCT_DEVEL_PREFIX)Rules
