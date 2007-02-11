# libxml2
DOWNLOAD=	ftp://xmlsoft.org/libxml2/libxml2-2.6.27.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/.libs/libxml2.so.2.6.26 $(NEWROOTDIR)/lib && \
		$(LN) -sf libxml2.so.2.6.26 $(NEWROOTDIR)/lib/libxml2.so.2 && \
		$(LN) -sf libxml2.so.2.6.26 $(NEWROOTDIR)/lib/libxml2.so && \
		$(CP) $(SRCDIR)/.libs/testdso.so.0.0.0 $(NEWROOTDIR)/lib && \
		$(LN) -sf testdso.so.0.0.0 $(NEWROOTDIR)/lib/testdso.so.0 && \
		$(LN) -sf testdso.so.0.0.0 $(NEWROOTDIR)/lib/testdso.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
