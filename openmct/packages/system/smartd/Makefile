# Smartmontools (Harddisk Monitoring)
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/smartmontools/smartmontools-5.37.tar.gz

INSTALL=	$(CP) $(SRCDIR)/{smartctl,smartd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{smartctl,smartd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
