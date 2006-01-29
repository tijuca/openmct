# PPTP
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/pptpclient/pptp-1.7.0.tar.gz

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc OPTIMIZE="-Os" PPPD="/sbin/pppd"

INSTALL=	$(CP) $(SRCDIR)/pptp $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/pptp

include		$(OPENMCT_DEVEL_PREFIX)Rules
