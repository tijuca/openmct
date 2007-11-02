# Linux Kernel
DOWNLOAD=	http://www.de.kernel.org/pub/linux/kernel/v2.4/linux-2.4.35.3.tar.bz2

MAKEARGS=	CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) vmlinux modules

INSTALL=	$(GZIP) -9 -c $(SRCDIR)/vmlinux.bin >$(OUTPUTDIR)/vmlinux.bin.gz && \
		cd $(SRCDIR) && make DEPMOD=/bin/true INSTALL_MOD_PATH=$(NEWROOTDIR) modules_install

include		$(OPENMCT_DEVEL_PREFIX)Rules
