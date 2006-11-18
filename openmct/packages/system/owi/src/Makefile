LANGUAGE=LANG_GERMAN
CFLAGS=-D$(LANGUAGE) -Os -Wall
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
BINARY=owi.cgi
LDFLAGS=-lcrypt

OBJECTS=ftp.o misc.o variable.o interface.o argument.o user.o group.o sysinfo.o file.o nfs.o shell.o owi.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(BINARY) $(OBJECTS)
	$(STRIP) -s $(BINARY)
	$(CP) $(BINARY) ../www

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm $(BINARY) $(OBJECTS) 2> /dev/null

install:
	$(CP) $(BINARY) ../www

uninstall:
	$(RM) ../www/$(BINARY)
