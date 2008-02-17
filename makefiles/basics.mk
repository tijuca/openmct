
# packet versions
BUSYBOX=1.7.2


# building the fs skeleton
cdkroot-skeleton:
	install -c -d $(CDKROOT)/bin
	install -c -d $(CDKROOT)/boot
	install -c -d $(CDKROOT)/dev
	install -c -d $(CDKROOT)/etc
	install -c -d $(CDKROOT)/include
	install -c -d $(CDKROOT)/mnt
	install -c -d $(CDKROOT)/lib
	install -c -d $(CDKROOT)/lib/pkgconfig
	install -c -d $(CDKROOT)/proc
	install -c -d $(CDKROOT)/root
	install -c -d $(CDKROOT)/sbin
	install -c -d $(CDKROOT)/tmp
	install -c -d $(CDKROOT)/var
	install -c -d $(CDKROOT)/var/etc
	install -c -d $(CDK)/$(TARGET)/usr/include
	install -c -d $(CDK)/$(TARGET)/usr/lib
# setting some symlinks
	ln -sf $(CDKROOT)/include	$(CDK)/$(TARGET)/include
	ln -sf $(CDKROOT)/lib		$(CDK)/$(TARGET)/lib

busybox: $(DOWNLOAD)/busybox-$(BUSYBOX)
	@if [ ! -f $(DEPS)/$@ ]; then \
	    echo -e "\033[1;33mbuild busybox\033[0m ..."; \
	    test -d $(APPS_BUILD_DIR)/busybox-$(BUSYBOX) || (bzcat $(DOWNLOAD)/busybox-$(BUSYBOX).tar.bz2 | /bin/tar -C $(APPS_BUILD_DIR) -xf -); \
	    (cp $(CONFIG)/busybox.config $(APPS_BUILD_DIR)/busybox-$(BUSYBOX)/.config && \
		cd $(APPS_BUILD_DIR)/busybox-$(BUSYBOX) && \
		make all install \
		CROSS_COMPILE=$(CDK)/bin/$(TARGET)- \
		ARCH=mipsel CONFIG_PREFIX=$(CDKROOT)); \
	    echo -e "\033[1;33mbuild busybox \033[0m... \033[1;32mDone!\033[0m"; \
	    touch $(DEPS)/$@ ; \
	else \
	    echo -e "\033[1;33mbuild busybox \033[0m... \033[1;32mallready builded!?!\033[0m"; \
	fi


$(DOWNLOAD)/busybox-$(BUSYBOX):
	test -f $(DOWNLOAD)/busybox-$(BUSYBOX).tar.bz2 || \
	        wget http://busybox.net/downloads/busybox-1.7.2.tar.bz2 -P $(DOWNLOAD)
