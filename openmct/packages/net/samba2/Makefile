# Samba 2.x
DOWNLOAD=	http://samba.org/samba/ftp/stable/samba-2.2.12.tar.gz

SRCDIRSUFFIX=	/source

CONFIGUREARGS=	--host=$(TARGET) \
		--with-fhs \
                --enable-shared \
                --enable-static \
                --disable-pie \
                --prefix=$(PREFIX) \
                --sysconfdir=/etc \
                --libdir=/var/etc/samba \
                --with-privatedir=/var/etc/samba \
                --with-piddir=/var/run \
                --localstatedir=/var \
                --with-netatalk \
                --with-syslog \
                --with-utmp \
                --with-readline \
                --with-libsmbclient \
                --with-winbind \
                --with-shared-modules=idmap_rid,idmap_ad \
                --with-msdfs \
                --with-automount \
                --with-tdbsam

INSTALL=	$(CP) $(SRCDIR)/source/bin/{smbd,nmbd,smbpasswd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{smbd,nmbd,smbpasswd} && \
		$(MKDIR) $(NEWROOTDIR)/var/etc/samba && \
		$(CP) $(SRCDIR)/examples/simple/smb.conf $(NEWROOTDIR)/var/etc/samba

include		$(OPENMCT_DEVEL_PREFIX)Rules
