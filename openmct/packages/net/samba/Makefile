# Samba 3.x
DOWNLOAD=	http://us1.samba.org/samba/ftp/stable/samba-3.0.26a.tar.gz

SRCDIRSUFFIX=	/source

CONFIGUREARGS=	--host=$(TARGET) \
		--with-fhs \
                --enable-shared \
                --enable-static \
                --disable-pie \
                --prefix=$(PREFIX) \
                --sysconfdir=/etc \
                --libdir=/etc/samba \
                --with-privatedir=/etc/samba \
                --with-piddir=/var/run/samba \
                --localstatedir=/var \
                --with-netatalk \
                --with-syslog \
                --with-utmp \
                --with-readline \
                --with-libsmbclient \
                --with-winbind \
                --with-shared-modules=idmap_rid \
                --with-msdfs \
                --with-automount \
                --with-tdbsam \
		--without-ldap

INSTALL=	$(CP) $(SRCDIR)/source/bin/{smbd,nmbd,smbpasswd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{smbd,nmbd,smbpasswd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
