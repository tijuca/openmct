# OpenSSL
DOWNLOAD=	http://www.openssl.org/source/openssl-0.9.8a.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --openssldir=/ shared openmct-mipsel

MAKEARGS=	PERL=/usr/bin/perl CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAG=-Os 

INSTALL=	$(CP) $(SRCDIR)/{libcrypto.so.0.9.8,libssl.so.0.9.8} $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/{libcrypto.so.0.9.8,libssl.so.0.9.8} && \
		$(LN) -sf libcrypto.so.0.9.8 $(NEWROOTDIR)/lib/libcrypto.so && \
		$(LN) -sf libssl.so.0.9.8 $(NEWROOTDIR)/lib/libssl.so && \
		cd $(SRCDIR) && make install_sw && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
