# libpng

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/libpng/libpng-1.2.20.tar.bz2	

DEPENDSON=	packages/lib/zlib

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared

INSTALL=        $(CP) $(SRCDIR)/.libs/libpng12.so $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/libpng12.so && \
                $(LN) -sf libpng12.so $(NEWROOTDIR)/lib/libpng.so && \
                cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
