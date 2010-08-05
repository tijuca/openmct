# hotplug

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

PREPARE= [ -f $(SRCDIR)/Makefile.in ] ||  (cd $(SRCDIR) && ./prepare.sh)

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --target=$(TARGET)-uclibc --enable-maintainer-mode --enable-debug --host=i686-pc-linux-gnu

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc RN=$(OPENMCT_CROSS_COMPILE)ranlib LD=$(OPENMCT_CROSS_COMPILE)ld

CLEAN=		cd src/ && rm -rf *.o hotplug

INSTALL=	$(CP) $(SRCDIR)/hotplug $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/hotplug

include		$(OPENMCT_DEVEL_PREFIX)Rules
