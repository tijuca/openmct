# NTP
DOWNLOAD=	http://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/ntp-4.2.0.tar.gz
CONFIGUREARGS=	--enable-all-clocks=no \
	        --without-crypto --without-electricfence --with-sntp \
	        --without-arlib

CONFIGUREENV=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc CFLAGS="-Os"

MAKEARGS=	-C ntpdate 

INSTALL=	$(CP) $(SRCDIR)/ntpdate/ntpdate $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ntpdate

include		$(OPENMCT_DEVEL_PREFIX)Rules
