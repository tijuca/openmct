# screen
DOWNLOAD=	ftp://ftp.gnu.org/gnu/screen/screen-4.0.2.tar.gz

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAGS="-Os"

INSTALL=	$(CP) $(SRCDIR)/screen $(NEWROOTDIR)/bin && \
                $(STRIP) $(NEWROOTDIR)/bin/screen

include		$(OPENMCT_DEVEL_PREFIX)Rules
