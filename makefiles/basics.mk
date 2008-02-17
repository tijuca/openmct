
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
	ln -sf $(CDKROOT)/include                    $(CDK)/$(TARGET)/include
	ln -sf $(CDKROOT)/lib                        $(CDK)/$(TARGET)/lib
