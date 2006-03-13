# Makefile.am for fcp

PATH := ${CCDIR}:$(PATH)
CC=$$HOME/openmct-build/cdk/mipsel-linux/bin/mipsel-linux-gcc
#CFLAGS = @CFLAGS@ @DEFS@ -I@top_srcdir@
AM_CFLAGS = $(CFLAGS)

prefix=@prefix@/cdk/mct-fs

bin_PROGRAMS    = fcp proccgi
fcp_SOURCES     = fcp.c
progcgi_SOURCES = proccgi.c
