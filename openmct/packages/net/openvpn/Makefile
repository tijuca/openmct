# OpenVPN
DOWNLOAD=	http://openvpn.net/release/openvpn-2.0.5.tar.gz

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --disable-lzo

DEPENDSON=	packages/lib/openssl

include		$(OPENMCT_DEVEL_PREFIX)Rules
