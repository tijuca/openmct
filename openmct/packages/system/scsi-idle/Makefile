# scsi-idle

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

BUILD=          $(OPENMCT_CROSS_COMPILE)gcc -Wall -Os -fomit-frame-pointer -pipe scsi-idle.c -o scsi-idle && \
		$(OPENMCT_CROSS_COMPILE)gcc -Wall -Os -fomit-frame-pointer -pipe scsi-start.c -o scsi-start

INSTALL=        $(CP) $(SRCDIR)/{scsi-idle,scsi-start} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{scsi-idle,scsi-start} && \
		$(MAKESYMLINKS) $(NEWROOTDIR) /sbin/scsi-start $(CONFIGDIR)/links

CLEAN=          $(RM) $(SRCDIR)/{scsi-idle,scsi-start}

include		$(OPENMCT_DEVEL_PREFIX)Rules
