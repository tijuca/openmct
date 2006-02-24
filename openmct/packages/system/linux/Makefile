# Linux Kernel
DOWNLOAD=	http://www.de.kernel.org/pub/linux/kernel/v2.4/linux-2.4.32.tar.bz2

MAKEARGS=	CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) vmlinux

INSTALL=	$(GZIP) -9 -c $(SRCDIR)/vmlinux.bin >$(OUTPUTDIR)/vmlinux.bin.gz
		
include		$(OPENMCT_DEVEL_PREFIX)Rules
