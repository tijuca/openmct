# IPTables (Firewalling)

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.netfilter.org/projects/iptables/files/iptables-1.3.7.tar.bz2

MAKEENV=	CC=$(OPENMCT_CROSS_COMPILE)gcc LD=$(OPENMCT_CROSS_COMPILE)ld

MAKEARGS=	COPT_FLAGS="-Os" KERNEL_DIR=$(PACKAGEDIR)/$(KERNEL) IPT_LIBDIR="/lib/iptables"

INSTALL=	$(CP) $(SRCDIR)/{iptables,iptables-restore,iptables-save} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{iptables,iptables-restore,iptables-save} && \
		$(MKDIR) $(NEWROOTDIR)/lib/iptables && \
		$(CP) $(SRCDIR)/extensions/*.so $(NEWROOTDIR)/lib/iptables && \
		$(STRIP) $(NEWROOTDIR)/lib/iptables/*.so

include		$(OPENMCT_DEVEL_PREFIX)Rules
