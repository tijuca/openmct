# e2fsprogs
DOWNLOAD=	http://www.kernel.org/pub/linux/kernel/people/tytso/e2fsprogs/e2fsprogs-1.41.3.tar.bz2

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-elf-shlibs

INSTALL=	$(CP) $(SRCDIR)/lib/libuuid.so.1.2 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libuuid.so.1.2 && \
		$(LN) -sf libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so.1 && \
		$(LN) -sf libuuid.so.1.2 $(NEWROOTDIR)/lib/libuuid.so && \
		$(CP) $(SRCDIR)/lib/libext2fs.so.2.4 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libext2fs.so.2.4 && \
		$(LN) -sf libext2fs.so.2.4 $(NEWROOTDIR)/lib/libext2fs.so.2 && \
		$(LN) -sf libext2fs.so.2.4 $(NEWROOTDIR)/lib/libext2fs.so && \
		$(CP) $(SRCDIR)/lib/libblkid.so.1.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libblkid.so.1.0 && \
		$(LN) -sf libblkid.so.1.0 $(NEWROOTDIR)/lib/libblkid.so.1 && \
		$(LN) -sf libblkid.so.1.0 $(NEWROOTDIR)/lib/libblkid.so && \
		$(CP) $(SRCDIR)/lib/libe2p.so.2.3 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libe2p.so.2.3 && \
		$(LN) -sf libe2p.so.2.3 $(NEWROOTDIR)/lib/libe2p.so.2 && \
		$(LN) -sf libe2p.so.2.3 $(NEWROOTDIR)/lib/libe2p.so && \
		$(CP) $(SRCDIR)/lib/et/libcom_err.so.2.1 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libcom_err.so.2.1 && \
		$(LN) -sf libcom_err.so.2.1 $(NEWROOTDIR)/lib/libcom_err.so.2 && \
		$(LN) -sf libcom_err.so.2.1 $(NEWROOTDIR)/lib/libcom_err.so && \
		$(CP) $(SRCDIR)/misc/mke2fs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/mke2fs && \
		$(CP) $(SRCDIR)/misc/tune2fs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/tune2fs && \
		cd $(SRCDIR) && make install-libs && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
