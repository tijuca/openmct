# e2fsprogs
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/e2fsprogs/e2fsprogs-1.38.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-elf-shlibs

INSTALL=	$(CP) $(SRCDIR)/lib/libuuid.so.1.2 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libuuid.so.1.2 && \
		$(LN) -sf $(NEWROOTDIR)/lib/libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so.1 && \
		$(LN) -sf $(NEWROOTDIR)/lib/libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so && \
		cd $(SRCDIR) && make install-libs && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
