# ncftp Client
DOWNLOAD=	ftp://ftp.ncftp.com/ncftp/ncftp-3.2.1-src.tar.bz2

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/bin/ncftp $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/ncftp

include		$(OPENMCT_DEVEL_PREFIX)Rules
