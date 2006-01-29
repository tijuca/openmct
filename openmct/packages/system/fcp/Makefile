# fcp
FILENAME=	fcp.c

BUILD=          $(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc -Os $(FILENAME) -o fcp

INSTALL=        $(CP) $(SRCDIR)/fcp $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/fcp

CLEAN=          $(RM) $(SRCDIR)/fcp

include		$(OPENMCT_DEVEL_PREFIX)Rules
