# ID3TAG
DOWNLOAD=	http://mesh.dl.sourceforge.net/sourceforge/mad/libid3tag-0.15.1b.tar.gz

CONFIGUREARGS=	--prefix=$(OUTPUTDIR)/$(TARGET) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/.libs/libid3tag.so.0.3.0 $(NEWROOTDIR)/lib && \
		$(STRIP) $(NEWROOTDIR)/lib/libid3tag.so.0.3.0 && \
		cd $(SRCDIR) && make install && cd ..

include		$(OPENMCT_DEVEL_PREFIX)Rules
