# Samba 3.x

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://samba.org/samba/ftp/stable/samba-3.2.6.tar.gz

SRCDIRSUFFIX=	/source

CONFIGUREENV+=  CONFIG_SITE=config.site

CONFIGUREARGS=	--host=$(TARGET) \
                --with-fhs \
                --enable-shared \
                --enable-static \
                --disable-pie \
                --prefix=$(PREFIX) \
                --sysconfdir=/etc \
                --libdir=/etc/samba \
                --with-privatedir=/etc/samba \
                --with-piddir=/var/run \
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

#INSTALL=	$(CP) $(SRCDIR)/source/bin/{smbd,nmbd,smbpasswd} $(NEWROOTDIR)/sbin && \
#		$(STRIP) $(NEWROOTDIR)/sbin/{smbd,nmbd,smbpasswd} && \
#		$(LN) -sf /var/etc/samba $(NEWROOTDIR)/etc/samba

include		$(OPENMCT_DEVEL_PREFIX)Rules
