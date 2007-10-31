LANGUAGE=LANG_GERMAN
CFLAGS=-D$(LANGUAGE) -Os -Wall -DFILE_BASE='"../examples"'
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
MKDIR=mkdir
LN=ln
BINARY=index.cgi
LDFLAGS=-lcrypt
DESTDIR=/www
CSS=css/openmct.css
IMAGES=images

OBJECTS=ftp.o misc.o variable.o argument.o user.o group.o \
	process.o file.o nfs.o interface.o smb.o rc.o harddisk.o \
	owi.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(BINARY) $(OBJECTS)
	$(STRIP) -s $(BINARY)

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(BINARY) $(OBJECTS) 2> /dev/null

install:
	$(MKDIR) -p $(DESTDIR)/cgi-bin
	$(CP) -a $(BINARY) $(DESTDIR)/cgi-bin
	$(MKDIR) -p $(DESTDIR)/css
	$(CP) -a $(CSS) $(DESTDIR)/css
	$(CP) -a $(IMAGES) $(DESTDIR)/

uninstall:
