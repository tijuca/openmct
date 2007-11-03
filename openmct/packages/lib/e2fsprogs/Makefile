# e2fsprogs
#DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/e2fsprogs/e2fsprogs-1.40.2.tar.gz
DOWNLOAD=	http://www.kernel.org/pub/linux/kernel/people/tytso/e2fsprogs/e2fsprogs-1.40.2.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-elf-shlibs

INSTALL=	$(CP) $(SRCDIR)/lib/libuuid.so.1.2 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libuuid.so.1.2 && \
		$(LN) -sf libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so.1 && \
		$(LN) -sf libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so && \
		$(CP) $(SRCDIR)/lib/libblkid.so.1.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libblkid.so.1.0 && \
		$(LN) -sf libblkid.so.1.0 $(NEWROOTDIR)/lib/libblkid.so.1 && \
		$(LN) -sf libblkid.so.1.0 $(NEWROOTDIR)/lib/libblkid.so && \
		$(CP) $(SRCDIR)/misc/mke2fs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/mke2fs && \
		$(CP) $(SRCDIR)/misc/tune2fs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/tune2fs && \
		cd $(SRCDIR) && make install-libs && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
