# proccgi
FILENAME=       proccgi.c

BUILD=          $(OPENMCT_CROSS_COMPILE)gcc -Os $(FILENAME) -o proccgi

INSTALL=        $(CP) $(SRCDIR)/proccgi $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/proccgi

CLEAN=          $(RM) $(SRCDIR)/proccgi

include         $(OPENMCT_DEVEL_PREFIX)Rules

