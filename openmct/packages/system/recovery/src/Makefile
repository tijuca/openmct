CFLAGS=-O2 -Wall -Wno-unused -fno-common -fno-schedule-insns -fno-schedule-insns2 -fno-strict-aliasing -msoft-float -Werror -mno-abicalls -fno-pic -mlong-calls
OUTPUT=recovery.o
SRC=recovery.c
CC=$(CROSS)gcc

all:
	$(CC) -D__KERNEL__ -DMODULE -I $(KERNEL_SOURCE)/include/ $(CFLAGS) -c $(SRC) -o $(OUTPUT)

install:

clean:
	rm $(OBJECT)
