# NZBget
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/nzbget/nzbget-0.2.3.tar.gz

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) \
		--with-libxml2-includes=$(OUTPUTDIR)/$(TARGET)/include/libxml2 \
		CPPFLAGS=-I$(OUTPUTDIR)/$(TARGET)/include/ncurses

INSTALL=	$(CP) $(SRCDIR)/nzbget $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/nzbget

include 	$(OPENMCT_DEVEL_PREFIX)Rules
