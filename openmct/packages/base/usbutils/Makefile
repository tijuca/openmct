# USB Utils
DOWNLOAD=	http://www.sfr-fresh.com/linux/misc/usbutils-0.73.tar.gz

DEPENDSON=	packages/lib/libusb

CONFIGUREARGS=  --prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) USBIDS_DIR=/var/share

INSTALL=	$(CP) $(SRCDIR)/lsusb $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/lsusb && \
		mkdir -p $(NEWROOTDIR)/var/share/ &&\
		$(CP) $(SRCDIR)/usb.ids $(NEWROOTDIR)/var/share/ && \
		$(TOUCH) $(INSTALLED)

include		$(OPENMCT_DEVEL_PREFIX)Rules
