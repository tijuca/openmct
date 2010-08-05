# NTP

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.eecis.udel.edu/~ntp/ntp_spool/ntp4/ntp-4.2/ntp-4.2.4p4.tar.gz

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) \
		--enable-all-clocks=no --without-arlib \
	        --without-crypto --without-electricfence --with-sntp

CONFIGUREENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc CFLAGS="-Os"

MAKEARGS=	-C ntpdate 

INSTALL=	$(CP) $(SRCDIR)/ntpdate/ntpdate $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ntpdate

include		$(OPENMCT_DEVEL_PREFIX)Rules
