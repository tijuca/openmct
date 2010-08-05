# DNRD (Domain Name Relay Daemon)

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/dnrd/dnrd-2.20.3.tar.gz

INSTALL=	$(CP) $(SRCDIR)/src/dnrd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/dnrd 

include		$(OPENMCT_DEVEL_PREFIX)Rules
