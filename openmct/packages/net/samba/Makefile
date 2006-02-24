# Samba 3.x
DOWNLOAD=	http://www.samba.org/samba/ftp/stable/samba-3.0.21b.tar.gz

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
                --with-shared-modules=idmap_rid,idmap_ad \
                --with-msdfs \
                --with-automount \
                --with-tdbsam

INSTALL=	$(CP) $(SRCDIR)/source/bin/{smbd,nmbd,smbpasswd} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{smbd,nmbd,smbpasswd}

include		$(OPENMCT_DEVEL_PREFIX)Rules
