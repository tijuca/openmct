# GNU Coreutils

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://alpha.gnu.org/gnu/coreutils//coreutils-5.3.0.tar.gz

INSTALL=	$(CP) $(SRCDIR)/src/su $(NEWROOTDIR)/bin && \
		$(STRIP) $(NEWROOTDIR)/bin/su && \
		$(CHMOD) 4775 $(NEWROOTDIR)/bin/su && \
		$(TOUCH) $(INSTALLED)

include		$(OPENMCT_DEVEL_PREFIX)Rules
