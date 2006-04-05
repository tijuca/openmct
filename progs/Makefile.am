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
#screen versiom
sver=4.0.2

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

all: bbox

bbox:
	cd $(ARCHIVDIR); \
	test -f busybox-$(bbver).tar.bz2 || wget http://www.busybox.net/downloads/busybox-$(bbver).tar.bz2
	test -d busybox-$(bbver) || tar xjvf $(ARCHIVDIR)/busybox-$(bbver).tar.bz2
	cp ../configs/busybox.config busybox-$(bbver)/.config
	cd busybox-$(bbver) && \
	    $(BUILDENV) \
	    $(MAKE) dep all && \
	    $(STRIP) busybox &&\
	    $(MAKE) install PREFIX=$(prefix) 

coreutils:
	cd $(ARCHIVDIR); \
	test -f coreutils-$(cuver).tar.gz || wget ftp://ftp.gnu.org/gnu/coreutils/coreutils-$(cuver).tar.gz
	test -d coreutils-$(cuver) || tar xzvf $(ARCHIVDIR)/coreutils-$(cuver).tar.gz
	cd coreutils-$(cuver) && \
	    $(BUILDENV)
	    ./configure \
		--prefix=$(prefix)/bin \
		--host=i686 && \
	    $(MAKE) && \
	    cp src/su $(prefix)/bin	

nano:
	cd $(ARCHIVDIR); \
	test -f nano-$(nanover).tar.gz || wget http://www.nano-editor.org/dist/v1.2/nano-$(nanover).tar.gz
	test -d nano-$(nanover) || tar xzvf $(ARCHIVDIR)/nano-$(nanover).tar.gz
	if [ ! -f nano-$(nanover)/.patched ]; then \
	    cd nano-$(nanover); \
	    patch -p1 < $(PATCHDIR)/nano.patch; \
	    touch .patched; \
	    fi; exit
	cd nano-$(nanover) && \
	    $(BUILDENV) \
	    ./configure \
	        --build=$(build) \
	        --host=$(target) && \
	    $(MAKE) CFLAGS+=-I$(CCDIR)/include/ncurses


ncurses:
	cd $(ARCHIVDIR); \
	test -f ncurses-$(ncursesver).tar.gz || wget ftp://ftp.gnu.org/pub/gnu/ncurses/ncurses-$(ncursesver).tar.gz
	test -d ncurses-$(ncursesver) || tar xzvf $(ARCHIVDIR)/ncurses-$(ncursesver).tar.gz
	cd ncurses-$(ncursesver); \
	./configure --build=$(build) \
		    --host=$(target) \
		    --prefix=$(CCDIR) \
		    --with-shared \
		    --disable-ext-funcs \
		    --with-progs \
		    $(BUILDENV) &&\
		    $(MAKE) && \
		    $(MAKE) install.includes && \
		    $(MAKE) install.libs && \
		    $(MAKE) install.man

screen:
	cd $(ARCHIVDIR); \
	test -f screen-$(sver).tar.gz || wget ftp://ftp.gnu.org/gnu/screen/screen-$(sver).tar.gz
	test -d screen-$(sver) || tar xzvf $(ARCHIVDIR)/screen-$(sver).tar.gz
	cd screen-$(sver); \
	./configure --build=$(build) \
		    --host=$(target) \
		    --prefix=$(prefix)/bin \
		    $(BUILDENV) && \
		    $(MAKE); \
	cp screen/screen $(prefix)/bin	


#CC=$(target)-gcc -I$(CCDIR)/include
	
