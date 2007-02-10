# OpenVPN
DOWNLOAD=	http://openvpn.net/release/openvpn-2.0.9.tar.gz

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --disable-lzo

DEPENDSON=	packages/lib/openssl

INSTALL=	$(CP) $(SRCDIR)/openvpn $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/openvpn

include		$(OPENMCT_DEVEL_PREFIX)Rules
