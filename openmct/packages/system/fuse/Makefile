# fuse

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/fuse/fuse-2.5.3.tar.gz

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --with-kernel=$(PACKAGEDIR)/system/linux/src

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc LD=$(OPENMCT_CROSS_COMPILE)ld

INSTALL=	$(CP) $(SRCDIR)/util/{mount.fuse,fusermount} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/fusermount && \
		($(MKDIR) -p $(NEWROOTDIR)/lib/modules/`$(UNAME) $(PACKAGEDIR)/system/linux/src`/kernel/fs/fuse ; \
		$(CP) $(SRCDIR)/kernel/fuse.o $(NEWROOTDIR)/lib/modules/`$(UNAME) $(PACKAGEDIR)/system/linux/src`/kernel/fs/fuse) && \
		$(CP) $(SRCDIR)/lib/.libs/libfuse.so.2.5.3 $(NEWROOTDIR)/lib && \
		$(LN) -sf libfuse.so.2.5.3 $(NEWROOTDIR)/lib/libfuse.so.2 && \
		$(LN) -sf libfuse.so.2.5.3 $(NEWROOTDIR)/lib/libfuse.so

include		$(OPENMCT_DEVEL_PREFIX)Rules
