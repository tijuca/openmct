# centericq

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://thekonst.net/download/centericq-4.21.0.tar.bz2

CONFIGUREENV=	EMPTY=EMPTY

CONFIGUREARGS=	--host=$(TARGET) \
		--prefix=$(PREFIX) \
		--with-curl=$(OPENMCT_CROSS_BASE)/bin/curl-config

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++

INSTALL=	$(CP) $(SRCDIR)/centericq $(NEWROOTDIR)/bin/centericq && \
		$(STRIP) $(NEWROOTDIR)/sbin/centericq

include		$(OPENMCT_DEVEL_PREFIX)Rules
