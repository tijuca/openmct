# Debootstrap
DOWNLOAD=	http://ftp.debian.org/debian/pool/main/d/debootstrap/debootstrap_1.0.3.tar.gz

BUILD=		$(OPENMCT_CROSS_COMPILE)gcc -Os \
		-o pkgdetails pkgdetails.c

INSTALL= 	$(MKDIR) -p $(NEWROOTDIR)/lib/debootstrap/scripts/debian ; \
		$(CP) $(SRCDIR)/pkgdetails $(NEWROOTDIR)/lib/debootstrap && \
		$(STRIP) $(NEWROOTDIR)/lib/debootstrap/pkgdetails && \
		$(CP) $(SRCDIR)/debootstrap $(NEWROOTDIR)/sbin && \
		$(CHMOD) 755 $(NEWROOTDIR)/sbin/debootstrap && \
		$(CP) $(SRCDIR)/devices.tar.gz $(NEWROOTDIR)/lib/debootstrap && \
		$(CP) $(SRCDIR)/functions $(NEWROOTDIR)/lib/debootstrap && \
		$(ECHO) "mipsel" > $(NEWROOTDIR)/lib/debootstrap/arch && \
		$(CP) $(SRCDIR)/scripts/debian/potato $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/scripts/debian/sarge $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/scripts/debian/sarge.buildd $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/scripts/debian/sid $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/scripts/debian/woody $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CP) $(SRCDIR)/scripts/debian/woody.buildd $(NEWROOTDIR)/lib/debootstrap/scripts && \
		$(CHMOD) 755 $(NEWROOTDIR)/lib/debootstrap/scripts/*

include		$(OPENMCT_DEVEL_PREFIX)Rules
