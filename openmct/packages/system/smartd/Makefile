# Smartmontools (Harddisk Monitoring)

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/smartmontools/smartmontools-5.37.tar.gz

INSTALL=	$(CP) $(SRCDIR)/{smartctl,smartd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{smartctl,smartd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
