# strace 

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://downloads.sourceforge.net/strace/strace-4.5.16.tar.bz2

INSTALL=	$(CP) $(SRCDIR)/strace $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/strace

include		$(OPENMCT_DEVEL_PREFIX)Rules
