# libgcrypt
DOWNLOAD=	ftp://ftp.gnupg.org/gcrypt/libgcrypt/libgcrypt-1.2.4.tar.bz2

DEPENDSON=	packages/lib/libgpg-error

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared \
		--with-gpg-error-prefix=$(OPENMCT_CROSS_BASE)

INSTALL=        $(CP) $(SRCDIR)/src/.libs/libgcrypt.so.11.2.3 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libgcrypt.so.11.2.3 && \
		$(LN) -sf libgcrypt.so.11.2.3 $(NEWROOTDIR)/lib/libgcrypt.so && \
		$(LN) -sf libgcrypt.so.11.2.3 $(NEWROOTDIR)/lib/libgcrypt.so.11 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
