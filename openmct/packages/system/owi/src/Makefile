LANGUAGE=LANG_GERMAN
CFLAGS=-D$(LANGUAGE) -Os -Wall
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
OUTPUT=../www
CP=cp
BINARY=owi.cgi
LDFLAGS=-lcrypt

OBJECTS=misc.o variable.o argument.o user.o group.o owi.o sysinfo.o file.o

all:	$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(BINARY) $(OBJECTS)
	$(STRIP) $(BINARY)
	$(CP) $(BINARY) ../www

%.o:	%.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm $(BINARY) $(OBJECTS) 2> /dev/null

install:
	$(CP) $(BINARY) ../www

uninstall:
	$(RM) ../www/$(BINARY)
