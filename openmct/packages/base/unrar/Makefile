# unrar
DOWNLOAD=	http://files5.rarlab.com/rar/unrarsrc-3.7.8.tar.gz

MAKEARGS=	-f makefile.unix CXX=$(OPENMCT_CROSS_COMPILE)g++ CXXFLAGS="-Os" STRIP=$(OPENMCT_CROSS_COMPILE)sstrip

INSTALL=	$(CP) $(SRCDIR)/unrar $(NEWROOTDIR)/bin 

include 	$(OPENMCT_DEVEL_PREFIX)Rules
