# NCurses

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.6.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --with-shared \
		--with-terminfo-dirs=/etc/terminfo \
		--with-default-terminfo-dir=/etc/terminfo

INSTALL=	$(CP) $(SRCDIR)/lib/libncurses.so.5.6 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libncurses.so.5.6 && \
		$(LN) -sf libncurses.so.5.6 $(NEWROOTDIR)/lib/libncurses.so.5 && \
		$(LN) -sf libncurses.so.5.6 $(NEWROOTDIR)/lib/libncurses.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
