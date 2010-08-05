# sigc++

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ftp.acc.umu.se/pub/GNOME/sources/libsigc++/2.1/libsigc++-2.1.1.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared 

INSTALL=	$(CP) $(SRCDIR)/sigc++/.libs/libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libsigc-2.0.so.0.0.0 && \
		$(LN) -sf libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib/libsigc-2.0.so.0 && \
		$(LN) -sf libsigc-2.0.so.0.0.0 $(NEWROOTDIR)/lib/libsigc-2.0.so && \
		cd $(SRCDIR) && make install && cd .. && \
		$(CP) $(SRCDIR)/sigc++config.h $(OUTPUTDIR)/$(TARGET)/include/sigc++-2.1

include		$(OPENMCT_DEVEL_PREFIX)Rules
