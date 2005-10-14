#
# MCT Firmware Builder
#
# Andi <andi@void.at>

MAKE=/usr/bin/make

ACTDIR=$(shell pwd)
KERNELDIR=$(ACTDIR)/kernel
EGCSDIR=$(ACTDIR)/egcs

all:	
	$(MAKE) -C $(KERNELDIR) preparekernel
	$(MAKE) -C $(EGCSDIR) KERNELDIR=$(KERNELDIR)
	$(MAKE) -C $(KERNELDIR) buildkernel
