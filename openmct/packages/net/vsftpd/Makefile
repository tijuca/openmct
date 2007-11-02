# VSFTPD
DOWNLOAD=	ftp://vsftpd.beasts.org/users/cevans/vsftpd-2.0.5.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc LIBS="-lcrypt"

INSTALL=	$(CP) $(SRCDIR)/vsftpd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/vsftpd && \
		$(ECHO) "listen=YES" > $(NEWROOTDIR)/etc/vsftpd.conf && \
		$(ECHO) "anonymous_enable=NO" >> $(NEWROOTDIR)/etc/vsftpd.conf && \
		$(ECHO) "local_enable=YES" >> $(NEWROOTDIR)/etc/vsftpd.conf && \
		$(ECHO) "write_enable=YES" >> $(NEWROOTDIR)/etc/vsftpd.conf && \
		$(ECHO) "background=YES" >> $(NEWROOTDIR)/etc/vsftpd.conf && \
		$(ECHO) "secure_chroot_dir=/var/empty" >> $(NEWROOTDIR)/etc/vsftpd.conf

include		$(OPENMCT_DEVEL_PREFIX)Rules
