# readline
DOWNLOAD=	ftp://ftp.gnu.org/gnu/readline/readline-5.2.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --enable-shared

INSTALL=        $(CP) $(SRCDIR)/shlib/{libhistory.so.5.2,libreadline.so.5.2} $(NEWROOTDIR)/lib && \
                $(STRIP) $(NEWROOTDIR)/lib/{libhistory.so.5.2,libreadline.so.5.2} && \
                cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
