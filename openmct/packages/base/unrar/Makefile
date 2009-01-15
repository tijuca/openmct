# unrar
OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.rarlab.com/rar/unrarsrc-3.8.5.tar.gz
MAKEARGS=	-f makefile.unix CXX=$(OPENMCT_CROSS_COMPILE)g++ CXXFLAGS="-Os" STRIP=$(OPENMCT_CROSS_COMPILE)sstrip

INSTALL=	$(CP) $(SRCDIR)/unrar $(NEWROOTDIR)/bin 

include 	$(OPENMCT_DEVEL_PREFIX)Rules
