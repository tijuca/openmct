LANGUAGE=LANG_GERMAN
CFLAGS=-D$(LANGUAGE) -Os -Wall
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
MKDIR=mkdir
LN=ln
BINARY=index.cgi
LDFLAGS=-lcrypt
DESTDIR=/www

OBJECTS=ftp.o misc.o variable.o interface.o argument.o user.o group.o sysinfo.o file.o nfs.o shell.o owi.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(BINARY) $(OBJECTS)
	$(STRIP) -s $(BINARY)

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm $(BINARY) $(OBJECTS) 2> /dev/null

install:
	$(MKDIR) -p $(DESTDIR)
	$(CP) -a $(BINARY) $(DESTDIR)

ose: misc.o variable.o argument.o file.o ose.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o ose misc.o variable.o argument.o file.o ose.o
	$(STRIP) -s ose

uninstall:
