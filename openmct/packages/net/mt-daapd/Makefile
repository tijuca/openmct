# mt-daapd
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/mt-daapd/mt-daapd-0.2.4.1.tar.gz	

DEPENDSON=	packages/lib/gdbm \
		packages/lib/id3tag \
		packages/lib/sqlite

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --enable-sqlite3

INSTALL=	$(CP) $(SRCDIR)/src/{mt-daapd,wavstreamer} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{mt-daapd,wavstreamer}

include		$(OPENMCT_DEVEL_PREFIX)Rules
