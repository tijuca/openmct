# scsi-idle

BUILD=          $(OPENMCT_CROSS_COMPILE)gcc -Wall -Os -fomit-frame-pointer -pipe scsi-idle.c -o scsi-idle && \
		$(OPENMCT_CROSS_COMPILE)gcc -Wall -Os -fomit-frame-pointer -pipe scsi-start.c -o scsi-start

INSTALL=        $(CP) $(SRCDIR)/{scsi-idle,scsi-start} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{scsi-idle,scsi-start} && \
		ln -s $(NEWROOTDIR)/sbin/scsi-start $(NEWROOTDIR)/sbin/scsi-stop

CLEAN=          $(RM) $(SRCDIR)/{scsi-idle,scsi-start}

include		$(OPENMCT_DEVEL_PREFIX)Rules
