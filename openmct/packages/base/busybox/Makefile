# Busybox (Basic Shell Utils)

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://busybox.net/downloads/busybox-1.17.2.tar.bz2

MAKEARGS=	CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) ARCH=mipsel 

INSTALL= 	cd $(SRCDIR) && make CROSS_COMPILE=$(OPENMCT_CROSS_COMPILE) ARCH=mipsel \
		CONFIG_PREFIX=$(NEWROOTDIR) install && cd .. && \
		mkdir -p $(NEWROOTDIR)/var/etc/ && \
		install -m 755 $(SRCDIR)/examples/udhcp/simple.script $(NEWROOTDIR)/var/etc/default.script
# mhh Script sollte statisch erstellt werden aus /var/etc/new
#		sed -e 's/etc/tmp/g' $(SRCDIR)/examples/udhcp/simple.script > $(SRCDIR)/examples/udhcp/test.script && \

include		$(OPENMCT_DEVEL_PREFIX)Rules
