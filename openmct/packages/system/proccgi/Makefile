# proccgi
FILENAME=       proccgi.c

BUILD=          $(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc -Os $(FILENAME) -o proccgi

INSTALL=        $(CP) $(SRCDIR)/proccgi $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/proccgi

CLEAN=          $(RM) $(SRCDIR)/proccgi

include         $(OPENMCT_DEVEL_PREFIX)Rules

