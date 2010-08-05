# upnp

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ovh.dl.sourceforge.net/sourceforge/pupnp/libupnp-1.6.3.tar.bz2

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/upnp/.libs/libupnp.so $(NEWROOTDIR)/lib && \
		$(CP) $(SRCDIR)/ixml/.libs/libixml.so $(NEWROOTDIR)/lib && \
		$(CP) $(SRCDIR)/threadutil/.libs/libthreadutil.so $(NEWROOTDIR) && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
