# Asterisk
DOWNLOAD=	http://ftp.digium.com/pub/asterisk/releases/asterisk-1.2.15.tar.gz

MAKEARGS=	CROSS_COMPILE_TARGET=$(OPENMCT_CROSS_BASE) \
		CROSS_COMPILE=$(OPENMCT_CROSS_BASE)/bin/$(TARGET)-uclibc- \
		OPTIMIZE="-Os -Dlinux" CROSS_ARCH=Linux

INSTALL=	

include		$(OPENMCT_DEVEL_PREFIX)Rules
