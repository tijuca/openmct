# Wireless Tools
DOWNLOAD=	http://pcmcia-cs.sourceforge.net/ftp/contrib/wireless_tools.27.tar.gz

MAKEARGS=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc

INSTALL=	$(CP) $(SRCDIR)/libiw.so.27 $(NEWROOTDIR)/lib && \
		$(LN) -sf $(NEWROOTDIR)/lib/libiw.so.27 $(NEWROOTDIR)/lib/libiw.so && \
		$(STRIP) $(NEWROOTDIR)/lib/libiw.so.27 && \
		$(CP) $(SRCDIR)/{ifrename,iwconfig,iwevent,iwgetid,iwlist,iwpriv,iwspy} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{ifrename,iwconfig,iwevent,iwgetid,iwlist,iwpriv,iwspy}

include		$(OPENMCT_DEVEL_PREFIX)Rules
