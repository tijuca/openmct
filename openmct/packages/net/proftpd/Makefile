# ProFTPD
DOWNLOAD=	ftp://ftp.proftpd.org/distrib/source/proftpd-1.2.10.tar.bz2

CONFIGUREARGS=	--disable-cap

INSTALL=	$(CP) $(SRCDIR)/{ftpcount,ftpdctl,ftpshut,ftptop,ftpwho,proftpd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{ftpcount,ftpdctl,ftpshut,ftptop,ftpwho,proftpd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
