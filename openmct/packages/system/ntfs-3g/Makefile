# ntfs-3g

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.ntfs-3g.org/ntfs-3g-1.5130.tgz

INSTALL=	$(CP) $(SRCDIR)/src/{ntfs-3g,ntfs-3g.probe} $(NEWROOTDIR)/bin && \
			$(LN) -sf /bin/ntfs-3g $(NEWROOTDIR)/sbin/mount.ntfs-3g && \
			$(CP) $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so.43.0.0 $(NEWROOTDIR)/lib && \
			$(LN) -sf libntfs-3g.so.43.0.0 $(NEWROOTDIR)/lib/libntfs-3g.so.43 && \
			$(LN) -sf libntfs-3g.so.43.0.0 $(NEWROOTDIR)/lib/libntfs-3g.so

include		$(OPENMCT_DEVEL_PREFIX)Rules
