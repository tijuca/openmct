# OpenSSL
DOWNLOAD=	http://www.openssl.org/source/openssl-0.9.8a.tar.gz

CONFIGUREARGS=	--prefix=/ shared openmct-mipsel

MAKEARGS=	PERL=/usr/bin/perl CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAG=-Os 

include		$(OPENMCT_DEVEL_PREFIX)Rules
