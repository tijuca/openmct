# vpnc

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.unix-ag.uni-kl.de/~massar/vpnc/vpnc-0.5.1.tar.gz

# DEPENDSON=      packages/lib/libgcrypt

MAKEARGS=       CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os '-DVERSION=\"$(shell cat $(SRCDIR)/VERSION)\"'" \
		LDFLAGS=-lgcrypt

include		$(OPENMCT_DEVEL_PREFIX)Rules
