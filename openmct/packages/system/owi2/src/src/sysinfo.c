/* sysinfo.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 *
 * system infos from the hd, network etc.
 */
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "owi2.h"

#define HARDDISKMODEL	"/proc/ide/ide0/hda/model"
#define HARDDISK		"/dev/ide/host0/bus0/target0/lun0/disc"

int get_hd_info(void) {
    system("hdparm -i "HARDDISK" > /tmp/hdinfo.tmp");
    FILE *i = fopen("/tmp/hdinfo.tmp", "r");
    unsigned char s1[256], s2[256], s3[256];
    int		  i1, i2, i3, pos, len;
    unsigned char buffer[4096];
    if (i) {
		while(fgets(buffer, 4096, i)) {
			switch (buffer[1]) {

			case 'P':	/* PIO-mode */
			case 'D':	/* DMA-mode */
			case 'U':	/* UDMA-mode */
		    /* find out which mode is in use. Normally this should be UDMA4, which is also know as UDMA66 */
		    /* the active mode is marked with an asterix in one of thoos three lines */
				len = strlen(buffer);
				pos = 12;
				if (len < (pos + 4))
					continue;
				while ((pos < (len - 4)) && (buffer[pos] != '*'))
					pos++;
					if (buffer[pos] != '*')
						continue;
					memset(s1, 0, 256);
					if (buffer[pos + 1] == 'p')
						memcpy(s1, buffer + pos + 1, 4);
					else {
						memcpy(s1, buffer + pos + 1, 5);
						s1[3] = toupper(s1[3]);
					}
					s1[0] = toupper(s1[0]);
					s1[1] = toupper(s1[1]);
					s1[2] = toupper(s1[2]);
					if (s1) {
						hd.tfmode = Strdup (s1);
					}
				break;

			case 'B':	/* BufferType & Size */
				i1 = 0;
				if (sscanf(buffer, " BuffType=%[^,], BuffSize=%dkB, MaxMultSect=%d, MultSect=%d\n", s1, &i1, &i2, &i3) != 4)
					continue;
				if (s1) {
					hd.cache = i1;
					hd.maxsc = i2;
					hd.maxscset = i3;
				}
				break;
			case 'M':	/* model, Firmware, Serial */
				memset(s1, 0, 256);
				memset(s2, 0, 256);
				memset(s3, 0, 256);
				if (sscanf(buffer, " Model=%[^,], FwRev=%[^,], SerialNo=%[^\n]", s1, s2, s3) != 3)
					continue;
				if (s1) {
					hd.modell = Strdup (s1);
				}

				if (s2) {
					hd.fw = Strdup(s2);
				}

				if (s3) {
					hd.serial = Strdup (s3);
				}
				break;
			default:
				break;
			}
		}
		fclose(i);
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] got following hddata\n"
											 "	hd.modell:\t%s\n"
											 "	hd.fw:\t\t%s\n"
											 "	hd.serial:\t%s\n"
											 "	hd.cache:\t%d kB\n"
											 "	hd.maxsc:\t%d\n"
											 "	hd.maxscset:\t%d\n"
											 "	hd.tfmode:\t%s\n"
											 ,__FUNCTION__,__LINE__
											 ,hd.modell
											 ,hd.fw
											 ,hd.serial
											 ,hd.cache
											 ,hd.maxsc
											 ,hd.maxscset
											 ,hd.tfmode);
    debug_info(debug_buf);
#endif
		return 1;
	}
	return -1;
}

unsigned char *read_hd_model (void) {
	FILE *model = NULL;
	char *ret = NULL;
	char buf[50];

	if ((model=fopen(HARDDISKMODEL, "r")) != NULL) {
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '"HARDDISKMODEL"' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
        if (fgets (buf, sizeof(buf), model) == NULL) {
            fclose(model);
            return "???";
        }
        buf[strlen(buf)-1]='\0';
#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] readed model: '%s' \n",__FUNCTION__,__LINE__,buf);
    debug_info(debug_buf);
#endif
        ret=Strdup(buf);
        fclose(model);
        return ret;
    }
    else

	return "???";
}
