# VSFTPD
DOWNLOAD=	ftp://vsftpd.beasts.org/users/cevans/vsftpd-2.0.3.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc LIBS="-lcrypt"

INSTALL=	$(CP) $(SRCDIR)/vsftpd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/vsftpd

include		$(OPENMCT_DEVEL_PREFIX)Rules
