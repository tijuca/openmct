# NFS Utils
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/nfs/nfs-utils-1.0.7.tar.gz

CONFIGUREARGS=	--disable-nfsv4 --disable-gss

INSTALL=	$(CP) $(SRCDIR)/utils/exportfs/exportfs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/exportfs && \
		$(CP) $(SRCDIR)/utils/nhfsstone/nhfsgraph $(NEWROOTDIR)/sbin && \
		$(CP) $(SRCDIR)/utils/nhfsstone/nhfsrun $(NEWROOTDIR)/sbin && \
		$(CP) $(SRCDIR)/utils/lockd/lockd $(NEWROOTDIR)/sbin/rpc.lockd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.lockd && \
		$(CP) $(SRCDIR)/utils/nfsd/nfsd $(NEWROOTDIR)/sbin/rpc.nfsd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.nfsd && \
		$(CP) $(SRCDIR)/utils/statd/statd $(NEWROOTDIR)/sbin/rpc.statd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.statd && \
		$(CP) $(SRCDIR)/utils/nfsstat/nfsstat $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/nfsstat && \
		$(CP) $(SRCDIR)/utils/nhfsstone/nhfsnums $(NEWROOTDIR)/sbin && \
		$(CP) $(SRCDIR)/utils/nhfsstone/nhfsstone $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/nhfsstone && \
		$(CP) $(SRCDIR)/utils/mountd/mountd $(NEWROOTDIR)/sbin/rpc.mountd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.mountd && \
		$(CP) $(SRCDIR)/utils/rquotad/rquotad $(NEWROOTDIR)/sbin/rpc.rquotad && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.rquotad && \
		$(CP) $(SRCDIR)/utils/showmount/showmount $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/showmount

include		$(OPENMCT_DEVEL_PREFIX)Rules
