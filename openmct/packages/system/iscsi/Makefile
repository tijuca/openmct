# iSCSI
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/unh-iscsi/unh_iscsi-1.7.00.tar.gz

MAKEARGS=	-f Makefile-24 CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) KERNEL_DIR=$(PACKAGEDIR)/system/linux/src CROSS_CFLAGS=

SRCDIRSUFFIX=	/src

CLEAN=		cd $(SRCDIR) && make clean

INSTALL=	$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/initiator/unh_iscsi_initiator.o \
		$(NEWROOTDIR)/lib/modules/`$(UNAME) $(PACKAGEDIR)/system/linux/src`/ && \
		$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/cmd/iscsi_config $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/iscsi_config && \
		$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/cmd/iscsi_manage $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/iscsi_manage && \
		$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/cmd/iscsi_test $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/iscsi_test && \
		$(CP) $(SRCDIR)$(SRCDIRSUFFIX)/cmd/parse $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/parse

include         $(OPENMCT_DEVEL_PREFIX)Rules
