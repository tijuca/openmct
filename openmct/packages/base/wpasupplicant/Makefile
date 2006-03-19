# wpa_supplicant
DOWNLOAD=	http://hostap.epitest.fi/releases/wpa_supplicant-0.4.8.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc

INSTALL=	$(CP) $(SRCDIR)/{wpa_cli,wpa_passphrase,wpa_supplicant} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{wpa_cli,wpa_passphrase,wpa_supplicant}

include		$(OPENMCT_DEVEL_PREFIX)Rules
