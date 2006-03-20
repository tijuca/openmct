# Makefile.am for basic programms

#busybox version
bbver=1.1.0

ARCHIVDIR=@prefix@/archives
STRIP=$$HOME/openmct-build/cdk/mipsel-linux/bin/mipsel-linux-sstrip
CCDIR=@prefix@/cdk/@target@/bin

AM_CFLAGS = $(CFLAGS)
PATH := $(CCDIR):$(PATH)
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

prefix=@prefix@/cdk/mct-fs


bin_PROGRAMS    = fcp proccgi
fcp_SOURCES     = fcp.c
proccgi_SOURCES = proccgi.c

all: bbox

bbox:
	cd $(ARCHIVDIR); \
	test -f busybox-$(bbver).tar.bz2 || wget http://www.busybox.net/downloads/busybox-1.1.0.tar.bz2
	test -d busybox-$(bbver) || tar xjvf $(ARCHIVDIR)/busybox-$(bbver).tar.bz2
	cp ../configs/busybox.config busybox-$(bbver)/.config
	cd busybox-$(bbver) && \
	    $(BUILDENV) \
	    $(MAKE) dep all && \
	    $(STRIP) busybox &&\
	    $(MAKE) install PREFIX=$(prefix) 
