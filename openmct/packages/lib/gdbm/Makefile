# GDBM
DOWNLOAD=	ftp://ftp.gnu.org/gnu/gdbm/gdbm-1.8.3.tar.gz

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/.libs/{libgdbm.so.3.0.0,libgdbm_compat.so.3.0.0} $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/{libgdbm.so.3.0.0,libgdbm_compat.so.3.0.0} && \
		$(LN) -sf libgdbm.so.3.0.0 $(NEWROOTDIR)/lib/libgdbm.so.3 && \
		$(LN) -sf libgdbm.so.3.0.0 $(NEWROOTDIR)/lib/libgdbm.so && \
		$(LN) -sf libgdbm_compat.so.3.0.0 $(NEWROOTDIR)/lib/libgdbm_compat.so.3 && \
		$(LN) -sf libgdbm_compat.so.3.0.0 $(NEWROOTDIR)/lib/libgdbm_compat.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
