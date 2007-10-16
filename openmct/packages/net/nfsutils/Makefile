# NFS Utils
#DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/nfs/nfs-utils-1.1.0.tar.gz
DOWNLOAD=	http://www.ch.kernel.org/pub/linux/utils/nfs/nfs-utils-1.1.0.tar.gz

DEPENDS_ON=	packages/net/tcpwrapper \
		packages/lib/e2fsprogs

CONFIGUREARGS=  --prefix=$(PREFIX) --host=$(TARGET) --disable-nfsv4 --disable-gss \
		--with-tcp-wrappers=$(PACKAGEDIR)/net/tcpwrapper/src

INSTALL=	$(CP) $(SRCDIR)/utils/exportfs/exportfs $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/exportfs && \
		$(CP) $(SRCDIR)/utils/nfsd/nfsd $(NEWROOTDIR)/sbin/rpc.nfsd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.nfsd && \
		$(CP) $(SRCDIR)/utils/statd/statd $(NEWROOTDIR)/sbin/rpc.statd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.statd && \
		$(CP) $(SRCDIR)/utils/nfsstat/nfsstat $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/nfsstat && \
		$(CP) $(SRCDIR)/utils/mountd/mountd $(NEWROOTDIR)/sbin/rpc.mountd && \
		$(STRIP) $(NEWROOTDIR)/sbin/rpc.mountd && \
		$(CP) $(SRCDIR)/utils/showmount/showmount $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/showmount

include		$(OPENMCT_DEVEL_PREFIX)Rules
