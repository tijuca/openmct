# source folder for OWI

CC=@CC@
AM_CFLAGS = @CFLAGS@ @DEFS@ -I@top_srcdir@ -I. -Wall

bindir=@OWI_HTMLDIR@/cgi-bin

# no transforming !! see http://www.linuxselfhelp.com/gnu/autoconf/html_chapter/autoconf_toc.html#TOC124
# it is just a quick hack
program_transform_name =

# the owi itself
bin_PROGRAMS = owi
owi_SOURCES = owi2.c debug.c read_form_data.c html.c pages.c functions.c

all:

clean:
	rm -f owi *.o

clean-all: clean
	rm -f Makefile.in Makefile ../.config

# so stuff for local testing, not realy needed for anybody else
# local
nfs: all
	cp owi @NFS_HTMLDIR@/cgi-bin