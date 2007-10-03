# Debootstrap
DOWNLOAD=	http://ftp.debian.org/debian/pool/main/d/debootstrap/debootstrap_1.0.3.tar.gz

BUILD=		$(OPENMCT_CROSS_COMPILE)gcc -Os \
		-o pkgdetails pkgdetails.c

INSTALL= 	$(MKDIR) $(NEWROOTDIR)/lib/debootstrap && \
		$(CP) $(SRCDIR)/pkgdetails $(NEWROOTDIR)/lib/debootstrap && \
		$(STRIP) $(NEWROOTDIR)/lib/debootstrap/pkgdetails && \
		$(CP) $(SRCDIR)/debootstrap $(NEWROOTDIR)/sbin && \
		$(CHMOD) 755 $(NEWROOTDIR)/sbin/debootstrap && \
		$(CP) $(SRCDIR)/devices.tar.gz $(NEWROOTDIR)/lib/debootstrap && \
		$(CP) $(SRCDIR)/functions $(NEWROOTDIR)/lib/debootstrap && \
		$(ECHO) "mipsel" > $(NEWROOTDIR)/lib/debootstrap/arch && \
		$(MKDIR) $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/potato $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/sarge $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/sarge.buildd $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/sid $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/sid.buildd $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/slink $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/woody $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/woody.buildd $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CHMOD) 755 $(NEWROOTDIR)/lib/debootstrap/scripts/*

include		$(OPENMCT_DEVEL_PREFIX)Rules
