# OpenMCT Webinterface (owi)
MAKEARGS=	CROSS=$(OPENMCT_CROSS_COMPILE)

INSTALL=	cd $(SRCDIR) && \
		make install DESTDIR=$(NEWVARDIR)/www

CLEAN=		cd $(SRCDIR) && make clean

include         $(OPENMCT_DEVEL_PREFIX)Rules
