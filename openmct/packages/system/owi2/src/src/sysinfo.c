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


int get_hd_info(void)
{
    system("hdparm -i "HARDDISK" > /tmp/hdinfo.tmp");
    FILE *i = fopen("/tmp/hdinfo.tmp", "r");
    unsigned char s1[256], s2[256], s3[256];
    int		  i1, i2, i3, pos, len, havemode=0, havesize=0;
    unsigned char buffer[1024];
    if (i)
    {
        while (fgets(buffer, sizeof(buffer), i))
        {
            switch (buffer[1])
            {

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
                else
                {
                    memcpy(s1, buffer + pos + 1, 5);
                    s1[3] = toupper(s1[3]);
                }
                s1[0] = toupper(s1[0]);
                s1[1] = toupper(s1[1]);
                s1[2] = toupper(s1[2]);
                if (s1)
                {
                    hd.tfmode = Strdup (s1);
                }
                break;

            case 'B':	/* BufferType & Size */
                i1 = 0;
                if (sscanf(buffer, " BuffType=%[^,], BuffSize=%dkB, MaxMultSect=%d, MultSect=%d\n", s1, &i1, &i2, &i3) != 4)
                    continue;
                if (s1)
                {
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
                if (s1)
                {
                    hd.modell = Strdup (s1);
                }

                if (s2)
                {
                    hd.fw = Strdup(s2);
                }

                if (s3)
                {
                    hd.serial = Strdup (s3);
                }
                break;
            default:
                break;
            }
        }
	fclose(i);
    system("rm -f /tmp/hdinfo.tmp");
	havemode=1;
    }

	FILE *geo = fopen("/proc/ide/ide0/hda/geometry", "r");
	if (geo)
	{
		fgets(buffer, sizeof(buffer), geo);
		// 					physical  cylinder/heads/sectors
		if (sscanf(buffer, "physical     %d/%d/%d\n", &hd.cyclinder, &hd.heads, &hd.sectors) == 3)
		{
			hd.size_netto = hd.cyclinder*hd.heads*hd.sectors*512/(1024*1024*1024);
			hd.size_brutto = hd.cyclinder*hd.heads*hd.sectors*512/(1000*1000*1000);
		}
	fclose(geo);
	havesize=1;
	}
	else
	{
#ifdef DEBUG
				snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open geo failed!\n",__FUNCTION__,__LINE__);
				debug_info(debug_buf);
#endif
	}
    // infos about partitions
    system("fdisk > /tmp/fdisk.tmp");
    FILE *fdisk = fopen("/tmp/fdisk.tmp", "r");

    if (fdisk)
    {
        hd.partition=0;
    // 1. searching for availbe partitions
        while (fgets(buffer, sizeof(buffer), fdisk))
        {
            if (buffer[0]=='/')
            {
                hd.partition++;
            }
        }
    // 2. file the structs with info
        fseek(fdisk, 0L, SEEK_SET); // jump back to the start if the fd
        while (fgets(buffer, sizeof(buffer), fdisk))
        {
            if (buffer[0]=='/')
            {
                int part=0, start=0, end=0, blocks=0, fs_id=0;
                if(sscanf(buffer, "/dev/ide/host0/bus0/target0/lun0/part%d %d %d %d %d Linux\n", &part, &start, &end, &blocks, &fs_id)==5);
                {
                    if (part==1)
                    {
                    //static struct hdpartitions hdpart[part];
                        if (fs_id != 82) // we have no swap, so size is greater 1GB
                            hd.size1=((end-start+1)*hd.heads*hd.sectors*.5)/(1024*1024); // size = (endblock - startblock+1) * hd.heads* hd.sectors * 512 / (1024*1024*1024)
                        else            // we have swap, size is less 1GB
                            hd.size1=((end-start+1)*hd.heads*hd.sectors*.5)/1024; // size = (endblock - startblock+1) * hd.heads* hd.sectors * 512 / (1024*1024)
                        hd.fs_id1=fs_id;
                    }
                    if (part==2)
                    {
                    //static struct hdpartitions hdpart[part];
                        if (fs_id != 82) // we have no swap, so size is greater 1GB
                            hd.size2=((end-start+1)*hd.heads*hd.sectors*.5)/(1024*1024); // size = (endblock - startblock+1) * hd.heads* hd.sectors * 512 / (1024*1024*1024)
                        else            // we have swap, size is less 1GB
                            hd.size2=((end-start+1)*hd.heads*hd.sectors*.5)/1024; // size = (endblock - startblock+1) * hd.heads* hd.sectors * 512 / (1024*1024)
                        hd.fs_id2=fs_id;
                    }
                }
            }
        }
    fclose (fdisk);
    system("rm -f /tmp/fdisk.tmp");
    }

    // searching the mountpoints etc.
    system("mount > /tmp/mount.tmp");
    FILE *mount = fopen("/tmp/mount.tmp", "r");

    if (mount)
    {
        while (fgets(buffer, sizeof(buffer), fdisk))
        {
            if (buffer[5]=='i')
            {
                int part=0;
                char directory[20], type[6];
/*                                 /dev/ide/host0/bus0/target0/lun0/part1  on /usr type ext3 (rw)*/
                if(sscanf(buffer, "/dev/ide/host0/bus0/target0/lun0/part%d on  %s  type %s   (\n", &part, directory, type)==3)
                {
                    if (part == 1)
                    {
                        hd.mountpoint1=Strdup(directory);
                        hd.fs1=Strdup(type);
                    }
                    if (part == 2)
                    {
                        hd.mountpoint2=Strdup(directory);
                        hd.fs2=Strdup(type);
                    }
                }
            }
        }
        fclose(mount);
        system("rm -f /tmp/mount.tmp");

    // getting the free space of the mountpoint, this is only to be done if we
    // have mountpoints, so we do this in the if() loop of the mountpoints
        snprintf(buffer, sizeof(buffer)-1,"df -h %s > /tmp/df.tmp",hd.mountpoint1);
        system(buffer);
        FILE *df = fopen("/tmp/df.tmp", "r");

        if (df)
        {
            int loop=0;
            while (fgets(buffer, sizeof(buffer), df))
            {
                loop++;
                char t1[8], t2[8];
                if (strncmp(buffer,"   ",3)==0) // ignoring all other lines who did't start with some spaces
                {
                    sscanf(buffer,"                       %s      %s    %s   %s\n",t1, t1, t1, t2);
                    hd.usage1=Strdup(t2);
                }
                else
                  hd.usage1=Strdup("??");
            }
        fclose(df);
        system("rm -f /tmp/df.tmp");
        }
    }

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
                 "	hd.size_netto:\t%d\n"
                 "	hd.size_brutto:\t%d\n"
                 "	hd.heads:\t%d\n"
                 "	hd.sectors:\t%d\n"
                 "	hd.partition:\t%d\n"
                 "	hd.size1:\t%d\n"
                 "	hd.fs_id1:\t%d\n"
                 ,__FUNCTION__,__LINE__
                 ,hd.modell
                 ,hd.fw
                 ,hd.serial
                 ,hd.cache
                 ,hd.maxsc
                 ,hd.maxscset
                 ,hd.tfmode
                 ,hd.size_netto
                 ,hd.size_brutto
                 ,hd.heads
                 ,hd.sectors
                 ,hd.partition
                 ,hd.size1
                 ,hd.fs_id1);
        debug_info(debug_buf);
#endif

	if ( (havemode==1) && (havesize==1))
		return 1;
	else if ( ((havemode==0) && (havesize==1)) ||((havemode==1) && (havesize==0)) )
		return 2;
	else
		return -1;
}

int get_networking_info (void)
{
    system ("ifconfig > /tmp/ifconfig.tmp");
    FILE *ifconfig = fopen("/tmp/ifconfig.tmp", "r");
    int loop=0;
    unsigned char buffer[1024];

    if (ifconfig)
    {
        while (fgets(buffer, sizeof(buffer), ifconfig))
            {
                if (strncmp(buffer,"eth",3)==0) // ignoring all other lines who did't start with 'eth'
                    loop++;
                    struct networking net[loop];
                    net[loop].iface=Strdup("128");
            }
#ifdef DEBUG
				snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] found %d entrys 'eth'\n",__FUNCTION__,__LINE__,loop);
				debug_info(debug_buf);
#endif

    fclose (ifconfig);
    system("rm -f /tmp/df.tmp");
    return 1;
    }

    return -1;
}

unsigned char *read_hd_model (void)
{
    FILE *model = NULL;
    char *ret = NULL;
    char buf[50];

    if ((model=fopen(HARDDISKMODEL, "r")) != NULL)
    {
#ifdef DEBUG
        extern char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '"HARDDISKMODEL"' o.k.\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
        if (fgets (buf, sizeof(buf), model) == NULL)
        {
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
