/***********************************************************
 * mct_flash.c
 *
 * local firmware flash utility for mct nas products
 * (for example http://www.claxan.de)
 *
 * Author: Andi <andi@void.at>
 *
 * Compile:
 * + create a cross compiler (you can skip this step if
 *   you have a mipsel plattform with a gcc)
 *   http://www.void.at/andi/papers/claxan_compile.txt
 * 
 * + compile mct_flash.c
 *   mipsel-uclibc-gcc mct_flash.c -o mct_flash
 *   mipsel-uclibc-strip mct_flash
 *
 * + upload mct_flash to your mct nas server
 *
 * + flash firmware
 *   # ./mct_flash [image filename]
 *   ...
 *
 *   # reboot
 *
 *
 * Greetz to Greuff, philipp and the other hoagie-fellas :-)
 *
 * THIS FILE IS FOR STUDYING PURPOSES ONLY AND A PROOF-OF-
 * CONCEPT. THE AUTHOR CAN NOT BE HELD RESPONSIBLE FOR ANY
 * DAMAGE DONE USING THIS PROGRAM.
 *
 ************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <asm/unistd.h>
#include <asm/mconfig.h>

_syscall2(int, read_config, int, type, void *, config)
_syscall2(int, write_config, int, type, void *, config)

static int Config_Read(int type, void *config)
{
        return read_config(type, config);
}

static int Config_Write(int type, void *config)
{
        return write_config(type, config);
}

int main(int argc, char **argv) {
   unsigned int fd;
   unsigned int r = 0;
   unsigned int offset = 0;
   struct {
      unsigned int    size;
      unsigned char   buf[8192];
   } sysbuf;

   if (argc != 1) {
      fd = open(argv[1], O_RDONLY);
      if (fd != -1) {
         printf("[*] '%s' successfully opened\n", argv[1]);

         printf("[*] start firmware update\n");
         Config_Write(MC_UPDATE_SYSTEM_START, 0);
    
         while ( (sysbuf.size = read(fd, sysbuf.buf, sizeof(sysbuf.buf))) != -1 && sysbuf.size != 0) {
  	    printf("[*] write buffer from offset 0x%08x\n", offset);
	    offset += sysbuf.size;
            Config_Write(MC_UPDATE_SYSTEM, &sysbuf);
         }

         printf("[*] stop firmware update\n");
         Config_Write(MC_UPDATE_SYSTEM_END, 0);

         close(fd);
      } else {
         fprintf(stderr, "[*] can't open '%s'\n", argv[1]);
         r = -1;
      }
   } else {
      fprintf(stderr, "usage: %s firmware\n", argv[0]);
   }
	
   return r;
}
