# ID3TAG

OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/mad/libid3tag-0.15.1b.tar.gz

DEPENDSON=	packages/lib/zlib

CONFIGUREARGS=	--prefix=$(OPENMCT_CROSS_BASE) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/.libs/libid3tag.so.0.3.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libid3tag.so.0.3.0 && \
		$(LN) -sf libid3tag.so.0.3.0 $(NEWROOTDIR)/lib/libid3tag.so.0 && \
		$(LN) -sf libid3tag.so.0.3.0 $(NEWROOTDIR)/lib/libid3tag.so && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
