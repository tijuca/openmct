# Dropbear (Tiny SSH Set)
DOWNLOAD=	http://matt.ucc.asn.au/dropbear/releases/dropbear-0.46.tar.bz2

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET) --disable-zlib

MAKEARGS=	PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" MULTI=1 SCPPROGRESS=1

INSTALL=	$(CP) $(SRCDIR)/dropbearmulti $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/dropbearmulti && \
		$(MAKESYMLINKS) $(NEWROOTDIR) /sbin/dropbearmulti $(CONFIGDIR)/links

include		$(OPENMCT_DEVEL_PREFIX)Rules
