# sigc++
DOWNLOAD=	http://ftp.gnome.org/pub/GNOME/sources/libsigc++/2.0/libsigc++-2.0.17.tar.bz2

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared 

INSTALL=	$(CP) $(SRCDIR)/sigc++/.libs/libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libsigc-2.0.so.0.0.0 && \
		$(LN) -sf libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib/libsigc-2.0.so.0 && \
		$(LN) -sf libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib/libsigc-2.0.so && \
		$(CP) $(SRCDIR)/sigc++config.h $(OUTPUTDIR)/$(TARGET)/include/sigc++-2.0 && \
		cd $(SRCDIR) && make install && cd .. 

include		$(OPENMCT_DEVEL_PREFIX)Rules
