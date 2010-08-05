# ez-ipupdate

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://www.ez-ipupdate.com/dist/ez-ipupdate-3.0.11b7.tar.gz

INSTALL=	$(CP) $(SRCDIR)/ez-ipupdate $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/ez-ipupdate

include		$(OPENMCT_DEVEL_PREFIX)Rules
