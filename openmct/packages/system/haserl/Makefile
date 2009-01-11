# haserl
OPENMCT_DEVEL_PREFIX ?= $(MY_OPENMCT_DEVEL_PREFIX)

DOWNLOAD=	http://downloads.sourceforge.net/haserl/haserl-0.9.24.tar.gz

CONFIGUREARGS=  --prefix=$(PREFIX) --host=$(TARGET) --target=$(TARGET)-uclibc --enable-bash-extensions 
#--enable-debug --host=i686-pc-linux-gnu

MAKEARGS=       CC=$(OPENMCT_CROSS_COMPILE)gcc RN=$(OPENMCT_CROSS_COMPILE)ranlib LD=$(OPENMCT_CROSS_COMPILE)ld


#CONFIGUREARGS=	--prefix=$(PREFIX) --host=$(TARGET)

INSTALL=	$(CP) $(SRCDIR)/src/haserl $(NEWROOTDIR)/sbin && \
		$(STRIP) $(NEWROOTDIR)/sbin/haserl

include		$(OPENMCT_DEVEL_PREFIX)Rules
