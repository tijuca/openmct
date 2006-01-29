# mii-diag
FILENAME=	mii-diag.c

BUILD=          $(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc -I. -O2 $(FILENAME) -o mii-diag

INSTALL=        $(CP) $(SRCDIR)/mii-diag $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/mii-diag

CLEAN=          $(RM) $(SRCDIR)/mii-diag

include		$(OPENMCT_DEVEL_PREFIX)Rules
