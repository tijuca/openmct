LANGUAGE=LANG_GERMAN
CFLAGS=-D$(LANGUAGE) -Os -Wall
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
MKDIR=mkdir
LN=ln
BINARY=owi.cgi
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
	$(MKDIR) -p $(DESTDIR)/cgi-bin
	$(CP) $(BINARY) $(DESTDIR)/cgi-bin
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/ftp.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/interface.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/user.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/group.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/sysinfo.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/nfs.cgi
	$(LN) -sf $(BINARY) $(DESTDIR)/cgi-bin/shell.cgi
	$(CP) -a ../www/* $(DESTDIR)

uninstall:
