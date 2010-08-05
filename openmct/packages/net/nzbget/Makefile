# NZBget

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/nzbget/nzbget-0.2.3.tar.gz

DEPENDSON=	packages/lib/libxml2

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc CXX=$(OPENMCT_CROSS_COMPILE)g++ \
		AR=$(OPENMCT_CROSS_COMPILE)ar RANLIB=$(OPENMCT_CROSS_COMPILE)ranlib \
		CFLAGS="-Os" CXXFLAGS="-Os"

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) \
		--with-libxml2-includes=$(OUTPUTDIR)/$(TARGET)/include/libxml2 \
		--with-stl-includes=$(OUTPUTDIR)/$(TARGET)/include \
		CPPFLAGS=-I$(OUTPUTDIR)/$(TARGET)/include/ncurses

INSTALL=	$(CP) $(SRCDIR)/nzbget $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/nzbget

include 	$(OPENMCT_DEVEL_PREFIX)Rules
