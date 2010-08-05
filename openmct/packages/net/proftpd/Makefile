# ProFTPD

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.proftpd.org/distrib/source/proftpd-1.3.0a.tar.bz2

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --disable-cap

INSTALL=	$(CP) $(SRCDIR)/{ftpcount,ftpdctl,ftpshut,ftptop,ftpwho,proftpd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{ftpcount,ftpdctl,ftpshut,ftptop,ftpwho,proftpd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
