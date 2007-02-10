# mt-daapd
DOWNLOAD=	http://nightlies.mt-daapd.org/dl.php?FILE=mt-daapd-svn-1498.tar.gz

DEPENDSON=	packages/lib/gdbm \
		packages/lib/id3tag \
		packages/lib/sqlite

CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET) --enable-sqlite3

INSTALL=	$(CP) $(SRCDIR)/src/{mt-daapd,wavstreamer} $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/{mt-daapd,wavstreamer}

include		$(OPENMCT_DEVEL_PREFIX)Rules
