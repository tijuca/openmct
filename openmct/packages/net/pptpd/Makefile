# PPTPD
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/poptop/pptpd-1.2.3.tar.gz

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc COPTS="-Os"

INSTALL=	$(CP) $(SRCDIR)/{pptpd,pptpctrl,bcrelay} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{pptpd,pptpctrl,bcrelay}

include 	$(OPENMCT_DEVEL_PREFIX)Rules
