# Portmap
DOWNLOAD=	ftp://ftp.porcupine.org/pub/security/portmap_4.tar.gz

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc WRAP_DIR=../../tcpwrapper/src

INSTALL=	$(CP) $(SRCDIR)/{portmap,pmap_dump,pmap_set} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{portmap,pmap_dump,pmap_set}

include		$(OPENMCT_DEVEL_PREFIX)Rules
