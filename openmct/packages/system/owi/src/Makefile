LANGUAGE=LANG_GERMAN
#CFLAGS=-D$(LANGUAGE) -Os -Wall -DFILE_BASE='""'
CFLAGS=-D$(LANGUAGE) -Os -Wall -DFILE_BASE='"../examples"'
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
MKDIR=mkdir
LN=ln
BINARY=index.cgi
INDEX=index.html
LDFLAGS=-lcrypt
DESTDIR=/www
CSS=css/openmct.css
IMAGES=images

OBJECTS=string.o array.o data.o file.o \
        ftp.o nfs.o smb.o interface.o \
	shell.o process.o harddisk.o \
	user.o group.o \
	owi.o 

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) -o $(BINARY) $(OBJECTS)
	$(STRIP) -s $(BINARY)

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

clean:
	@rm -f $(BINARY) $(OBJECTS) 2> /dev/null

install:
	$(MKDIR) -p $(DESTDIR)/cgi-bin
	$(CP) -a $(BINARY) $(DESTDIR)/cgi-bin
	$(MKDIR) -p $(DESTDIR)/css
	$(CP) $(CSS) $(DESTDIR)/css
	$(MKDIR) -p $(DESTDIR)/images
	$(CP) $(IMAGES)/* $(DESTDIR)/images
	$(CP) $(INDEX) $(DESTDIR)

uninstall:
