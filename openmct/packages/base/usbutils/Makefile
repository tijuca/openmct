# USB Utils

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://downloads.sourceforge.net/project/linux-usb/usbutils/0.73/usbutils-0.73.tar.gz

DEPENDSON=	packages/lib/libusb

CONFIGUREARGS=  --prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) USBIDS_DIR=/var/share

INSTALL=	$(CP) $(SRCDIR)/lsusb $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/lsusb && \
		mkdir -p $(NEWROOTDIR)/var/share/ &&\
		$(CP) $(SRCDIR)/usb.ids $(NEWROOTDIR)/var/share/ && \
		$(TOUCH) $(INSTALLED)

include		$(OPENMCT_DEVEL_PREFIX)Rules
