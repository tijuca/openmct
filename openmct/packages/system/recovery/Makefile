# OpenMCT Recovery Kernel Module

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

MAKEARGS=	CROSS=$(OPENMCT_CROSS_COMPILE) KERNEL_SOURCE=$(PACKAGEDIR)/system/linux/src

INSTALL=	$(CP) $(SRCDIR)/recovery.o \
		$(NEWROOTDIR)/lib/modules/`$(UNAME) $(PACKAGEDIR)/system/linux/src`/

CLEAN=		cd $(SRCDIR) && make clean

include         $(OPENMCT_DEVEL_PREFIX)Rules
