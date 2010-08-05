# wpa_supplicant

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://hostap.epitest.fi/releases/wpa_supplicant-0.6.0.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc

INSTALL=	$(CP) $(SRCDIR)/{wpa_cli,wpa_passphrase,wpa_supplicant} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{wpa_cli,wpa_passphrase,wpa_supplicant}

include		$(OPENMCT_DEVEL_PREFIX)Rules
