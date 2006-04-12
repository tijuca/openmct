# Makefile.am for basic programms

srcdir = @srcdir@
VPATH = @srcdir@


#busybox version
bbver=1.1.0
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

ARCHIVDIR=@prefix@/archives
PATCHDIR=../../patches
#STRIP=$$HOME/openmct-build/cdk/mipsel-linux/bin/mipsel-linux-sstrip
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

all: bbox coreutils nano mp3info

bbox:
	cd $(ARCHIVDIR); \
	test -f busybox-$(bbver).tar.bz2 || wget http://www.busybox.net/downloads/busybox-$(bbver).tar.bz2
	test -d busybox-$(bbver) || tar xjvf $(ARCHIVDIR)/busybox-$(bbver).tar.bz2
	cp ../configs/busybox.config busybox-$(bbver)/.config
	cd busybox-$(bbver) && \
	    $(BUILDENV) \
	    $(MAKE) dep all && \
	    $(STRIP) busybox && \
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
