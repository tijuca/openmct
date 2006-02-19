# NCurses
DOWNLOAD=	http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.5.tar.gz

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --with-shared

INSTALL=	$(CP) $(SRCDIR)/lib/libncurses.so.5.5 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libncurses.so.5.5 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
