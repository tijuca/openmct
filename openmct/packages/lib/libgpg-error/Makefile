# libgpg-error

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://ftp.gnupg.org/gcrypt/libgpg-error/libgpg-error-1.5.tar.bz2

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared

INSTALL=        $(CP) $(SRCDIR)/src/.libs/libgpg-error.so.0.3.1 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libgpg-error.so.0.3.1 && \
		$(LN) -sf libgpg-error.so.0.3.1 $(NEWROOTDIR)/lib/libgpg-error.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
