# ushare

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ushare.geexbox.org/releases/ushare-1.1a.tar.bz2

CONFIGUREARGS=	--prefix=/ --cross-compile --with-libupnp-dir=$(OPENMCT_CROSS_LIB)

DEPENDSON=	packages/lib/gettext \
		packages/lib/libupnp

INSTALL=	$(CP) $(SRCDIR)/src/ushare $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ushare

include		$(OPENMCT_DEVEL_PREFIX)Rules
