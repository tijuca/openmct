# RSync

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://rsync.samba.org/ftp/rsync/rsync-3.0.5.tar.gz

INSTALL=	$(CP) $(SRCDIR)/rsync $(NEWROOTDIR)/sbin && \
            $(STRIP) $(NEWROOTDIR)/sbin/rsync

include		$(OPENMCT_DEVEL_PREFIX)Rules
