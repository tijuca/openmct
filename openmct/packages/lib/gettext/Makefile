# gettext

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ftp.gnu.org/gnu/gettext/gettext-0.16.1.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os"

INSTALL=	cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
