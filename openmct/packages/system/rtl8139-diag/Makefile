# rtl8139-diag
FILENAME=	rtl8139-diag.c

BUILD=          $(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc -I. -O2 $(FILENAME) -o rtl8139-diag

INSTALL=        $(CP) $(SRCDIR)/rtl8139-diag $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/rtl8139-diag

CLEAN=          $(RM) $(SRCDIR)/rtl8139-diag

include		$(OPENMCT_DEVEL_PREFIX)Rules
