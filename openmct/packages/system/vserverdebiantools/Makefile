# VServer Debiantools
DOWNLOAD=	http://ftp.debian.org/debian/pool/main/v/vserver-debiantools/vserver-debiantools_0.1.10.tar.gz

BUILD=

INSTALL= 	$(CP) $(SRCDIR)/dupvserver.sh $(NEWROOTDIR)/sbin/dupvserver &&  \
		$(CP) $(SRCDIR)/newnfsvserver.sh $(NEWROOTDIR)/sbin/newnfsvserver &&  \
		$(CP) $(SRCDIR)/newvserver.sh $(NEWROOTDIR)/sbin/newvserver &&  \
		$(CP) $(SRCDIR)/stripserver.sh $(NEWROOTDIR)/sbin/stripserver &&  \
		$(CHMOD) 755 $(NEWROOTDIR)/sbin/{dupvserver,newnfsvserver,newvserver,stripserver}

include		$(OPENMCT_DEVEL_PREFIX)Rules
