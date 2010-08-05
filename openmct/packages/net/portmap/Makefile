# Portmap

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.porcupine.org/pub/security/portmap_4.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc WRAP_DIR=../../tcpwrapper/src

INSTALL=	$(CP) $(SRCDIR)/{portmap,pmap_dump,pmap_set} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{portmap,pmap_dump,pmap_set}

include		$(OPENMCT_DEVEL_PREFIX)Rules
