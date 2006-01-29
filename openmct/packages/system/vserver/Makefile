# VServer
DOWNLOAD=	http://www.13thfloor.at/vserver/s_release/v1.2.10/util-vserver-0.30.tar.bz2

CONFIGUREENV=	CC=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-gcc \
		CXX=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-g++ \
		LD=$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-ld \
		CFLAGS="-Os"

CONFIGUREARGS=	--with-kerneldir=../../linux/src --disable-internal-headers

INSTALL=	$(CP) $(SRCDIR)/src/capchroot			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/chbind			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/chcontext			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/fakerunlevel		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/filetime			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/ifspec			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/listdevip			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/parserpmdump		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/readlink			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/rebootmgr			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/reducecap			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/setattr			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/showattr			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/showperm			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vbuild			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vcheck			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vdu				$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vfiles			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vlimit			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vreboot			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vserver-stat		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/src/vunify			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/rootshell		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/save_s_context		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vprofile		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vpstree			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vrpm			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vserver-copy		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vserverkillall		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vservers.grabinfo.sh	$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vsysvwrapper		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vtop			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vunify.sh		$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/vserver			$(NEWROOTDIR)/sbin/ && \
		$(CP) $(SRCDIR)/scripts/util-vserver-vars	$(NEWROOTDIR)/sbin/ && \
		$(OUTPUTDIR)/$(TARGET)/bin/$(TARGET)-uclibc-sstrip		\
		$(NEWROOTDIR)/sbin/capchroot	$(NEWROOTDIR)/sbin/chbind	\
		$(NEWROOTDIR)/sbin/chcontext	$(NEWROOTDIR)/sbin/fakerunlevel	\
		$(NEWROOTDIR)/sbin/filetime	$(NEWROOTDIR)/sbin/ifspec	\
		$(NEWROOTDIR)/sbin/listdevip	$(NEWROOTDIR)/sbin/parserpmdump	\
		$(NEWROOTDIR)/sbin/readlink	$(NEWROOTDIR)/sbin/rebootmgr	\
		$(NEWROOTDIR)/sbin/reducecap	$(NEWROOTDIR)/sbin/setattr	\
		$(NEWROOTDIR)/sbin/showattr	$(NEWROOTDIR)/sbin/showperm	\
		$(NEWROOTDIR)/sbin/vbuild	$(NEWROOTDIR)/sbin/vcheck	\
		$(NEWROOTDIR)/sbin/vdu		$(NEWROOTDIR)/sbin/vfiles	\
		$(NEWROOTDIR)/sbin/vlimit	$(NEWROOTDIR)/sbin/vreboot	\
		$(NEWROOTDIR)/sbin/vserver-stat	$(NEWROOTDIR)/sbin/vunify

UNINSTALL=	$(RM) $(NEWROOTDIR)/sbin/capchroot		&& \
		$(RM) $(NEWROOTDIR)/sbin/chbind			&& \
		$(RM) $(NEWROOTDIR)/sbin/chcontext		&& \
		$(RM) $(NEWROOTDIR)/sbin/fakerunlevel		&& \
		$(RM) $(NEWROOTDIR)/sbin/filetime		&& \
		$(RM) $(NEWROOTDIR)/sbin/ifspec			&& \
		$(RM) $(NEWROOTDIR)/sbin/listdevip		&& \
		$(RM) $(NEWROOTDIR)/sbin/parserpmdump		&& \
		$(RM) $(NEWROOTDIR)/sbin/readlink		&& \
		$(RM) $(NEWROOTDIR)/sbin/rebootmgr		&& \
		$(RM) $(NEWROOTDIR)/sbin/reducecap		&& \
		$(RM) $(NEWROOTDIR)/sbin/setattr		&& \
		$(RM) $(NEWROOTDIR)/sbin/showattr		&& \
		$(RM) $(NEWROOTDIR)/sbin/showperm		&& \
		$(RM) $(NEWROOTDIR)/sbin/vbuild			&& \
		$(RM) $(NEWROOTDIR)/sbin/vcheck			&& \
		$(RM) $(NEWROOTDIR)/sbin/vdu			&& \
		$(RM) $(NEWROOTDIR)/sbin/vfiles			&& \
		$(RM) $(NEWROOTDIR)/sbin/vlimit			&& \
		$(RM) $(NEWROOTDIR)/sbin/vreboot		&& \
		$(RM) $(NEWROOTDIR)/sbin/vserver-stat		&& \
		$(RM) $(NEWROOTDIR)/sbin/vunify			&& \
		$(RM) $(NEWROOTDIR)/sbin/rootshell		&& \
		$(RM) $(NEWROOTDIR)/sbin/save_s_context		&& \
		$(RM) $(NEWROOTDIR)/sbin/vprofile		&& \
		$(RM) $(NEWROOTDIR)/sbin/vpstree		&& \
		$(RM) $(NEWROOTDIR)/sbin/vrpm			&& \
		$(RM) $(NEWROOTDIR)/sbin/vserver-copy		&& \
		$(RM) $(NEWROOTDIR)/sbin/vserverkillall		&& \
		$(RM) $(NEWROOTDIR)/sbin/vservers.grabinfo.sh	&& \
		$(RM) $(NEWROOTDIR)/sbin/vsysvwrapper		&& \
		$(RM) $(NEWROOTDIR)/sbin/vtop			&& \
		$(RM) $(NEWROOTDIR)/sbin/vunify.sh		&& \
		$(RM) $(NEWROOTDIR)/sbin/vserver		&& \
		$(RM) $(NEWROOTDIR)/sbin/util-vserver-vars

include		$(OPENMCT_DEVEL_PREFIX)Rules
