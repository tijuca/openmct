# USB Utils librarys
DOWNLOAD=	http://downloads.sourceforge.net/libusb/libusb-0.1.12.tar.gz

CONFIGUREARGS=  --prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) 

INSTALL=	cd $(SRCDIR) && make install && cd .. &&\
                $(CP) $(SRCDIR)/.libs/libusb-0.1.so.4.4.4 $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/libusb-0.1.so.4.4.4 && \
                $(LN) -sf libusb-0.1.so.4.4.4 $(NEWROOTDIR)/lib/libusb-0.1.so.4 && \
                $(LN) -sf libusb-0.1.so.4.4.4 $(NEWROOTDIR)/lib/libusb.so && \
                $(CP) $(SRCDIR)/.libs/libusbpp-0.1.so.4.4.4 $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/libusbpp-0.1.so.4.4.4 && \
                $(LN) -sf libusbpp-0.1.so.4.4.4 $(NEWROOTDIR)/lib/libusbpp-0.1.so.4 && \
                $(LN) -sf libusbpp-0.1.so.4.4.4 $(NEWROOTDIR)/lib/libusbpp.so		

include		$(OPENMCT_DEVEL_PREFIX)Rules
