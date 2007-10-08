# boost
DOWNLOAD=	http://downloads.sourceforge.net/boost/boost_1_34_1.tar.bz2

#OPENMCT_DEVEL_PREFIX=/home/carsten/openmct/trunk/openmct/

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CXX=$(OPENMCT_CROSS_COMPILE)g++ 

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --exec-prefix=$(OPENMCT_CROSS_BASE) --with-libraries=date_time filesystem thread regex program_options 

# propably more of theese libs are not nessesary
# date_time, filesystem, graph, iostreams, program_options, python, regex, serialization, signals, test, thread, wave, 

INSTALL=	make -C src/ install

include		$(OPENMCT_DEVEL_PREFIX)Rules
