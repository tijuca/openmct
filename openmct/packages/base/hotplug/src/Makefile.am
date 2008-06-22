
CC=@CC@
AM_CFLAGS = @CFLAGS@ @DEFS@ -I@top_srcdir@ -Wall

bin_PROGRAMS = hotplug
hotplug_SOURCES = hotplug.c output.c functions.c config.c
#all: hotplug

#hotplug: hotplug.o

all:

clean:
	rm -f hotplug *.o

nfs: all
	cp hotplug /home/carsten/openmct/trunk/openmct/output/nfs/sbin/
