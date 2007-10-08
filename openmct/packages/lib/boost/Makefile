# boost
DOWNLOAD=	http://downloads.sourceforge.net/boost/boost_1_34_1.tar.bz2

#OPENMCT_DEVEL_PREFIX=/home/carsten/openmct/trunk/openmct/

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ 
#		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1/sigc++" \
#		CXXFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1 -I$(OPENMCT_CROSS_INCLUDE)/sigc++-2.1/sigc++"

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --with-libraries=date_time filesystem thread regex program_options 

# propably mor of theese libs are not nessesary
# date_time, filesystem, graph, iostreams, program_options, python, regex, serialization, signals, test, thread, wave, 

#--host=$(TARGET) 
#--enable-shared --disable-aligned 
#\
#		--with-openssl=$(OPENMCT_CROSS_BASE)

#INSTALL=	$(CP) $(SRCDIR)/src/.libs/libtorrent.so.9.0.0 $(NEWROOTDIR)/lib && \
#		$(STRIP) $(NEWROOTDIR)/lib/libtorrent.so.9.0.0 && \
#		$(LN) -sf libtorrent.so.9.0.0 $(NEWROOTDIR)/lib/libtorrent.so.6 && \
#		$(LN) -sf libtorrent.so.9.0.0 $(NEWROOTDIR)/lib/libtorrent.so && \
#		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
