# Makefile.am for basic programms

srcdir = @srcdir@
VPATH = @srcdir@


#busybox version
bbver=1.1.2
#corutils version
cuver=5.93
#nano version
nanover=1.2.5
#ncurses version
ncursesver=5.5
#screen version
sver=4.0.2
#mp3info version
mp3infover=0.8.4
#nfsutils version
nfsutilsver=1.0.7
#tcp_wrappers version
tcp_wver=7.6
#dropbear version
dropver=0.48.1
#wpasupplicant version
wpaver=0.4.8
#smartmontools version
smt_ver=5.33

ARCHIVDIR=@prefix@/archives
PATCHDIR=../../patches
CCDIR=@prefix@/cdk/@target@

AM_CFLAGS = $(CFLAGS)
PATH := $(CCDIR)/bin:$(PATH)
BUILDENV := AR=$(target)-ar \
            AS=$(target)-as \
	    CC=$(target)-gcc \
	    CXX=$(target)-g++ \
	    NM=$(target)-nm \
	    STRIP=$(target)-sstrip \
	    RANLIB=$(target)-ranlib \
	    CFLAGS="$(TARGET_CFLAGS)" \
	    CXXFLAGS="$(TARGET_CFLAGS)" \
	    LDFLAGS="$(TARGET_LDFLAGS)"

prefix=@prefix@/cdk


bin_PROGRAMS    = fcp proccgi
fcp_SOURCES     = fcp.c
proccgi_SOURCES = proccgi.c

system: bbox coreutils nano mp3info smartd
network: portmap tcp_wrappers

bbox:
	cd $(ARCHIVDIR); \
	test -f busybox-$(bbver).tar.bz2 || wget http://www.busybox.net/downloads/busybox-$(bbver).tar.bz2
	test -d busybox-$(bbver) || tar xjvf $(ARCHIVDIR)/busybox-$(bbver).tar.bz2
	cp ../configs/busybox.config busybox-$(bbver)/.config
	cd busybox-$(bbver) && \
	    $(BUILDENV) \
	    $(MAKE) dep all && \
	    $(MAKE) install PREFIX=$(prefix)/mct-fs 

coreutils:
	cd $(ARCHIVDIR); \
	test -f coreutils-$(cuver).tar.gz || wget ftp://ftp.gnu.org/gnu/coreutils/coreutils-$(cuver).tar.gz
	test -d coreutils-$(cuver) || tar xzvf $(ARCHIVDIR)/coreutils-$(cuver).tar.gz
	cd coreutils-$(cuver) && \
	    $(BUILDENV) \
	    ./configure \
		--prefix=$(prefix)/mct-fs/bin \
		--host=i686 && \
	    $(MAKE) && \
	    cp src/su $(prefix)/mct-fs/bin	

nano:
	cd $(ARCHIVDIR); \
	test -f nano-$(nanover).tar.gz || wget http://www.nano-editor.org/dist/v1.2/nano-$(nanover).tar.gz
	test -d nano-$(nanover) || tar xzvf $(ARCHIVDIR)/nano-$(nanover).tar.gz
	if [ ! -f nano-$(nanover)/.patched ]; then \
	    cd nano-$(nanover); \
	    patch -p1 < $(PATCHDIR)/nano.patch; \
	    touch .patched; \
	fi;
	cd nano-$(nanover) && \
	    $(BUILDENV) \
	    ./configure \
	        --build=$(build) \
	        --host=$(target) && \
	    $(MAKE) CFLAGS+=-I$(CCDIR)/include/ncurses

mp3info:
	cd $(ARCHIVDIR); \
	test -f mp3info-$(mp3infover).tgz || wget ftp://ftp.ibiblio.org/pub/linux/apps/sound/mp3-utils/mp3info/mp3info-$(mp3infover).tgz
	test -d mp3info-$(mp3infover) || tar xzvf $(ARCHIVDIR)/mp3info-$(mp3infover).tgz
	if [ ! -f mp3info-$(mp3infover)/.patched ]; then \
	    cd mp3info-$(mp3infover); \
	    patch -p1 < $(PATCHDIR)/mp3info.patch; \
	    touch .patched; \
	fi;
	cd mp3info-$(mp3infover) && \
	    $(MAKE) CFLAGS+="-Os" mp3info \
	    $(BUILDENV) && \
	cp mp3info $(prefix)/mct-fs/sbin

tcp_wrappers:
	cd $(ARCHIVDIR); \
	test -f tcp_wrappers_$(tcp_wver).tar.gz || wget ftp://ftp.porcupine.org/pub/security/tcp_wrappers_$(tcp_wver).tar.gz
	test -d tcp_wrappers_$(tcp_wver) || tar xzvf $(ARCHIVDIR)/tcp_wrappers_$(tcp_wver).tar.gz
	if [ ! -f tcp_wrappers_$(tcp_wver)/.patched ]; then \
	    cd tcp_wrappers_$(tcp_wver); \
	    patch -p1 < $(PATCHDIR)/tcp_wrappers.patch; \
	    touch .patched; \
	fi;
	cd tcp_wrappers_$(tcp_wver) && \
	    $(BUILDENV) \
	    $(MAKE) CC=mipsel-linux-gcc REAL_DAEMON_DIR=/tmp linux
	    

nfsutils:
	cd $(ARCHIVDIR); \
	test -f nfs-utils-$(nfsutilsver).tar.gz || wget http://mesh.dl.sourceforge.net/sourceforge/nfs/nfs-utils-$(nfsutilsver).tar.gz
	test -d nfs-utils-$(nfsutilsver) || tar xzvf $(ARCHIVDIR)/nfs-utils-$(nfsutilsver).tar.gz
	if [ ! -f nfs-utils-$(nfsutilsver)/.patched ]; then \
	    cd nfs-utils-$(nfsutilsver); \
	    patch -p1 < $(PATCHDIR)/nfsutils.patch; \
	    touch .patched; \
	fi;
	cd nfs-utils-$(nfsutilsver) && \
	    $(BUILDENV) \
	    ./configure \
		--build=$(build) \
	        --host=$(target) \
		--prefix=$(prefix)/mct-fs/sbin \
		--disable-gss \
		--disable-nfsv4 &&\
	    $(MAKE) CFLAGS+="-I$(CCDIR)/include/ -I./support/include/"
	

smartd:
	cd $(ARCHIVDIR); \
	test -f smartmontools-$(smt_ver).tar.gz || wget http://mesh.dl.sourceforge.net/sourceforge/smartmontools/smartmontools-$(smt_ver).tar.gz
	test -d smartmontools-$(smt_ver) || tar xzvf $(ARCHIVDIR)/smartmontools-$(smt_ver).tar.gz
	cd smartmontools-$(smt_ver) && \
	    $(BUILDENV) \
	    ./configure \
		--build=$(build) \
	        --host=$(target) \
		--prefix=$(prefix)/mct-fs \
		--with-docdir=$(prefix)/tmp \
		--with-mandir=$(prefix)/tmp && \
	    $(MAKE) install
	rm -rf $(prefix)/tmp




portmap: tcp_wrappers
	cd $(ARCHIVDIR); \
	test -f portmap_4.tar.gz || wget ftp://ftp.porcupine.org/pub/security/portmap_4.tar.gz
	test -d portmap_4 || tar xzvf $(ARCHIVDIR)/portmap_4.tar.gz
	if [ ! -f portmap_4/.patched ]; then \
	    cd portmap_4; \
	    patch -p1 < $(PATCHDIR)/portmap.patch; \
	    touch .patched; \
	fi;
	cd portmap_4 && \
	    $(BUILDENV) \
	    $(MAKE) CC=mipsel-linux-gcc && \
	    cp pmap_dump pmap_set portmap $(prefix)/mct-fs/sbin
	

dropbear:
	cd $(ARCHIVDIR); \
	test -f dropbear-$(dropver).tar.gz || wget http://matt.ucc.asn.au/dropbear/releases/dropbear-$(dropver).tar.gz
	test -d dropbear-$(dropver) || tar xzvf $(ARCHIVDIR)/dropbear-$(dropver).tar.gz
	if [ ! -f dropbear-$(dropver)/.patched ]; then \
	    cd dropbear-$(dropver); \
	    patch -p1 < $(PATCHDIR)/dropbear.patch; \
	    touch .patched; \
	fi;
	cd dropbear-$(dropver) && \
	    $(BUILDENV) \
	    ./configure \
		--build=$(build) \
	        --host=$(target) \
		--prefix=$(prefix)/mct-fs/sbin \
		--disable-zlib && \
	    $(MAKE) PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" \
	    MULTI=1 \
	    SCPPROGRESS=1 &&\
	    cp dropbearmulti $(prefix)/mct-fs/sbin &&\
	    cd $(prefix)/mct-fs/sbin && \
	    for i in dropbear dropbearkey dropbearconvert; do \
		ln -s dropbearmulti $$i ; \
	    done && \
	    cd $(prefix)/mct-fs/bin && \
	    for i in dbclient scp ssh ; do \
		ln -s ../sbin/dropbearmulti $$i ; \
	    done




#wpasupplicant:
#	cd $(ARCHIVDIR); \
#	test -f wpa_supplicant-$(wpaver).tar.gz || wget http://hostap.epitest.fi/releases/wpa_supplicant-$(wpaver).tar.gz
#	test -d wpa_supplicant-$(wpaver) || tar xzvf $(ARCHIVDIR)/wpa_supplicant-$(wpaver).tar.gz
#	if [ ! -f wpa_supplicant-$(wpaver)/.patched ]; then \
#	    cd wpa_supplicant-$(wpaver); \
#	    patch -p1 < $(PATCHDIR)/wpa.patch; \
#	    touch .patched; \
#	fi
#	cd wpa_supplicant-$(wpaver) && \
#	    $(MAKE) $(BUILDENV) CFLAGS+=-Os CPPFLAGS+=-I$(CCDIR)/include LIBS+=-L$(CCDIR)/lib
#	    

screen:
	cd $(ARCHIVDIR); \
	test -f screen-$(sver).tar.gz || wget ftp://ftp.gnu.org/gnu/screen/screen-$(sver).tar.gz
	test -d screen-$(sver) || tar xzvf $(ARCHIVDIR)/screen-$(sver).tar.gz
	cd screen-$(sver); \
	    $(BUILDENV) \
	    ./configure \
		--build=$(build) \
		--host=$(target) \
		--prefix=$(prefix)/bin \
	    $(MAKE); \
	cp screen/screen $(prefix)/bin	
