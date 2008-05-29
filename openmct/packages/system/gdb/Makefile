# gdb

DOWNLOAD=	http://ftp.gnu.org/gnu/gdb/gdb-6.3.tar.gz	

CONFIGUREARGS=	--prefix=$(PREFIX) \
 	        --build=$(build) \
 	        --host=$(TARGET) \
 	        --target=$(TARGET) \
 	        --disable-nls \
 	        --without-uiout --disable-gdbmi \
 	        --disable-tui --disable-gdbtk --without-x \
 	        --without-included-gettext \
 	        --enable-threads \
 	        --disable-dependency-tracking \
 	        --disable-libtool-lock 

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ \
		CFLAGS="-Os" CXXFLAGS="-Os" \
		AR=$(OPENMCT_CROSS_COMPILE)ar \
		LD=$(OPENMCT_CROSS_COMPILE)ld

DEPENDSON=	packages/lib/ncurses

INSTALL=	$(CP) $(SRCDIR)/gdb/gdb $(NEWROOTDIR)/sbin && \
		$(CP) $(SRCDIR)/gdb/gbdserver/gdbserver $(NEWROOTDIR)/sbin \
		$(CP) $(OPENMCT_DEVEL_PREFIX)/$(DEPENDSON)/lib/libncurses.* $(NEWROOTDIR)/lib

include		$(OPENMCT_DEVEL_PREFIX)Rules
