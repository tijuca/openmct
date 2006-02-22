# nano
DOWNLOAD=	http://www.nano-editor.org/dist/v1.2/nano-1.2.5.tar.gz

CONFIGUREENV=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc \
		CFLAGS="-Os -I$(OUTPUTDIR)/$(TARGET)/include  -I$(OUTPUTDIR)/$(TARGET)/include/ncurses"

INSTALL=	$(CP) $(SRCDIR)/nano $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/nano

include		$(OPENMCT_DEVEL_PREFIX)Rules
