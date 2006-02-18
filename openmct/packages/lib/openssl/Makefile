# OpenSSL
DOWNLOAD=	http://www.openssl.org/source/openssl-0.9.8a.tar.gz

CONFIGUREARGS=	--prefix=/ shared openmct-mipsel

MAKEARGS=	PERL=/usr/bin/perl CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAG=-Os 

INSTALL=	$(CP) $(SRCDIR)/{libcrypto.so.0.9.8,libssl.so.0.9.8} $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/{libcrypto.so.0.9.8,libssl.so.0.9.8}

include		$(OPENMCT_DEVEL_PREFIX)Rules
