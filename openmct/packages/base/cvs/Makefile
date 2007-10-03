# cvs
DOWNLOAD=	http://ftp.gnu.org/non-gnu/cvs/source/stable/1.11.22/cvs-1.11.22.tar.bz2

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc \
		CFLAGS="-Os -I$(OPENMCT_CROSS_INCLUDE)"

INSTALL=	$(CP) $(SRCDIR)/src/cvs $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/cvs

include		$(OPENMCT_DEVEL_PREFIX)Rules
