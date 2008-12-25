# owi2

OPENMCT_DEVEL_PREFIX=/home/carsten/openmct/trunk/openmct/

PREPARE= [ -f $(SRCDIR)/Makefile.in ] ||  (cd $(SRCDIR) && ./prepare.sh)

CONFIGUREARGS=	--prefix=$(OUTPUTDIR) --target=$(TARGET)-uclibc --enable-maintainer-mode --enable-debug --host=i686-pc-linux-gnu --enable-strip=$(OPENMCT_CROSS_COMPILE)strip

MAKEARGS=	CC=$(OPENMCT_CROSS_COMPILE)gcc RN=$(OPENMCT_CROSS_COMPILE)ranlib LD=$(OPENMCT_CROSS_COMPILE)ld

DEPENDSON=	packages/system/smartd

INSTALL=	mkdir -p $(NEWROOTDIR)/var/www/ && \
		cd src/ && make install && \
		$(STRIP) $(NEWROOTDIR)/var/www/cgi-bin/owi

#		$(CP) $(SRCDIR)/owi $(NEWROOTDIR)/var/www/ && \
#		$(STRIP) $(NEWROOTDIR)/var/www/owi

include		$(OPENMCT_DEVEL_PREFIX)Rules
