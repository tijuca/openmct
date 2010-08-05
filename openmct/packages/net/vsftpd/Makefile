# VSFTPD

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	ftp://vsftpd.beasts.org/users/cevans/vsftpd-2.0.5.tar.gz

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc LIBS="-lcrypt"

INSTALL=	$(CP) $(SRCDIR)/vsftpd $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/vsftpd && \
		$(ECHO) "listen=YES" > $(NEWROOTDIR)/var/etc/vsftpd.conf && \
		$(ECHO) "anonymous_enable=NO" >> $(NEWROOTDIR)/var/etc/vsftpd.conf && \
		$(ECHO) "local_enable=YES" >> $(NEWROOTDIR)/var/etc/vsftpd.conf && \
		$(ECHO) "write_enable=YES" >> $(NEWROOTDIR)/var/etc/vsftpd.conf && \
		$(ECHO) "background=YES" >> $(NEWROOTDIR)/var/etc/vsftpd.conf && \
		$(ECHO) "secure_chroot_dir=/var/run/vsftpd" >> $(NEWROOTDIR)/var/etc/vsftpd.conf

include		$(OPENMCT_DEVEL_PREFIX)Rules
