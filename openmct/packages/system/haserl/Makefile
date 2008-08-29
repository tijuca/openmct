# haserl
OPENMCT_DEVEL_PREFIX=/home/carsten/openmct/trunk/openmct/
DOWNLOAD=	http://downloads.sourceforge.net/haserl/haserl-0.9.24.tar.gz

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/src/haserl $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/haserl

include		$(OPENMCT_DEVEL_PREFIX)Rules
