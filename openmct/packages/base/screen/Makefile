# screen
DOWNLOAD=	ftp://ftp.gnu.org/gnu/screen/screen-4.0.2.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os"

INSTALL=	$(CP) $(SRCDIR)/screen $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/screen

include		$(OPENMCT_DEVEL_PREFIX)Rules
