# ncftp Client

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.ncftp.com/ncftp/ncftp-3.2.2-src.tar.bz2

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/bin/ncftp $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/ncftp

include		$(OPENMCT_DEVEL_PREFIX)Rules
