AUTOMAKE_OPTION = foreign

CC=@CC@
AM_CFLAGS = @CFLAGS@ @DEFS@ -I@top_srcdir@ -Wall
SUBDIRS = img css html src

bindir=@OWI_HTMLDIR@/cgi-bin

# no transforming !! see http://www.linuxselfhelp.com/gnu/autoconf/html_chapter/autoconf_toc.html#TOC124
# it is just a quick hack
#program_transform_name =

all:

clean:
	rm -f *.o
	cd src && rm -f *.o

clean-all: clean
	rm -f Makefile.in Makefile .config

# so stuff for local testing, not realy needed for anybody else
# local
