# gettext
DOWNLOAD=	http://ftp.gnu.org/gnu/gettext/gettext-0.14.5.tar.gz	

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --enable-shared

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAGS="-Os"

INSTALL=	cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
