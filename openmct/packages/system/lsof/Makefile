# LSOF 
DOWNLOAD=	ftp://lsof.itap.purdue.edu/pub/tools/unix/lsof/lsof_4.77.tar.bz2

EXTRACTPOST=	$(TAR) -C $(SRCDIR) -xf $(SRCDIR)/lsof_4.77_src.tar

CONFIGUREARGS=	-n linux

SRCDIRSUFFIX=	/lsof_4.77_src

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os"

INSTALL=	$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/lsof $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/lsof

include		$(OPENMCT_DEVEL_PREFIX)Rules
