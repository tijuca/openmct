# sqlite

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.sqlite.org/sqlite-3.3.12.tar.gz

DEPENDSON=	packages/lib/readline

#CONFIGUREENV=	TARGET_READLINE_INC=$(OPENMCT_CROSS_BASE)/include/readline \
#		CC=$(OPENMCT_CROSS_COMPILE)gcc CXX=$(OPENMCT_CROSS_COMPILE)g++ CFLAGS="-Os" CXXFLAGS="-Os"

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET) --with-shared

MAKEARGS=	BCC="gcc -Os"

INSTALL=	$(CP) $(SRCDIR)/.libs/libsqlite3.so.0.8.6 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libsqlite3.so.0.8.6 && \
		$(LN) -sf libsqlite3.so.0.8.6 $(NEWROOTDIR)/lib/libsqlite3.so.0 && \
		$(LN) -sf libsqlite3.so.0.8.6 $(NEWROOTDIR)/lib/libsqlite3.so && \
		$(CP)  $(SRCDIR)/.libs/sqlite3 $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/sqlite3 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
