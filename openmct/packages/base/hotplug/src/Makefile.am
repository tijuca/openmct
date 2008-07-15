
CC=@CC@
AM_CFLAGS = @CFLAGS@ @DEFS@ -I@top_srcdir@ -Wall

# local
#NFS_PATH=../../../output/nfs/sbin/

#SVN
NFS_PATH=../../../../output/nfs/sbin/

bin_PROGRAMS = hotplug
hotplug_SOURCES = hotplug.c output.c functions.c config.c
#all: hotplug

#hotplug: hotplug.o

all:

clean:
	rm -f hotplug *.o

nfs: all
	cp hotplug $(NFS_PATH)
