/* -*- mode: C; c-file-style: "gnu" -*- */
/* sysinfo.c General system information
 *
 * Copyright (C) 2006 OpenMCT
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sys.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/sysinfo.h"

/* \fn sysinfo_main(argc, argv)
 * General system information
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int sysinfo_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   
   /* Print header information */
   owi_header("User Management");
   
   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print user list */
      sysinfo_list();
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn sysinfo_list()
 * Show system information
 */
void sysinfo_list() {
   sysinfo_general();
   
   printf("<table>\n"
          "<tr><td valign=\"top\">\n");
    
   sysinfo_filesystems();
 
   printf("</td><td>&nbsp;</td><td valign=\"top\">\n");

   sysinfo_network();

   printf("</td></tr>\n"
          "</table>\n");
}

/* \fn sysinfo_general()
 * Display general system information like swap, ram, uptime ...
 */
void sysinfo_general() {
   /* System information */
   struct sysinfo si;

   /* Get system information */
   if (!sysinfo(&si)) {
      owi_headline(1, SYSINFO_HEADLINE);
      owi_headline(2, SYSINFO_DESCRIPTION);
      printf("<table class=\"%s\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\">\n"
             "<thead>\n"
             "<tr>\n"
             "<th>Load (1 / 5 / 15 minute load avarage)</th>\n"
             "<th>Memory (All / Free / Shared)</th>\n"
             "<th>Speicher in Puffern:</t>\n"
             "<th>Swap (All / Free):</th>\n"
             "<th>Tasks</th>\n"
             "</tr>\n"
             "</thead>\n"
             "<tr onmouseover=\"this.className='%s';\""
             " onmouseout=\"this.className='%s';\">\n"
             "<td>%u / %u / %u</td>\n"
             "<td>%u MB / %u MB / %u MB</td>\n"
             "<td>%u</td>\n"
             "<td>%u MB / %u MB</td>\n"
             "<td>%d</td>\n"
             "</tr>\n"
             "</table>\n",
             CONTENT_TABLE_BOX_CLASS,
             CONTENT_TABLE_CLASS_MOUSEOVER,
             CONTENT_TABLE_CLASS_MOUSEOUT,
             (unsigned int)si.loads[0], (unsigned int)si.loads[1],
	     (unsigned int)si.loads[2], (unsigned int)(si.totalram / 1024 / 1024),
             (unsigned int)(si.freeram / 1024 / 1024),
             (unsigned int)(si.sharedram / 1024 / 1024),
             (unsigned int)si.bufferram,
             (unsigned int)(si.totalswap / 1024 / 1024),
             (unsigned int)(si.freeswap / 1024 / 1024),
             (unsigned int)si.procs);
   }
}

/* \fn sysinfo_filesystems()
 * Display information about mounted devices (like filesystem, diskspace ...)
 */
void sysinfo_filesystems() {
   /* File handler */
   FILE *fp = NULL;
   /* Mount information */
   char device[256], mountpoint[256], fs[256];

   /* Try to open mount information file */
   fp = fopen("/proc/mounts", "r");
   /* Ok? */
   if (fp) {
      owi_headline(1, SYSINFO_FILESYSTEMS);
      owi_headline(2, SYSINFO_FILESYSTEMS_DESCRIPTION);
      printf("<table class=\"%s\" cellpadding=\"0\" cellspacing=\"0\" align=\"left\">\n"
             "<thead>\n"
             "<tr>\n"
             "<th>Ger&auml;t</th>\n"
             "<th>Eingeh&auml;ngt auf</th>\n"
             "<th>Dateisystem</th>\n"
             "</tr>\n"
             "</thead>\n",
             CONTENT_TABLE_BOX_CLASS);
      /* Read until file end reached */
      while (fscanf(fp, "%s %s %s", device, mountpoint, fs) == 3) {
         if (strcasecmp(fs, "0")) {
            printf("<tr onmouseover=\"this.className='%s';\""
                      " onmouseout=\"this.className='%s';\">\n"
                   "<td>%s</td>\n"
                   "<td>%s</td>\n"
                   "<td>%s</td>\n"
                   "</tr>\n",
                   CONTENT_TABLE_CLASS_MOUSEOVER,
                   CONTENT_TABLE_CLASS_MOUSEOUT,
                   device,
                   mountpoint,
                   fs);
         }
      }
      printf("</table>\n");
      /* Close */
      fclose(fp);
   }
}

/* \fn sysinfo_network()
 * Display network devices information
 */
void sysinfo_network() {
   /* File handler */
   FILE *fp = NULL;
   /* Device information */
   char device[1024], line[1024];
   /* Trafic information */
   int in, out;

   /* Try to open network information file */
   fp = fopen("/proc/net/dev", "r");
   /* Ok? */
   if (fp) {
      owi_headline(1, SYSINFO_NETWORK_DEVICES);
      owi_headline(2, SYSINFO_NETWORK_DEVICES_DESCRIPTION);
      printf("<table class=\"%s\" cellpadding=\"0\" cellspacing=\"0\" align=\"left\">\n"
             "<thead>\n"
             "<tr>\n"
             "<th>Interface</th>\n"
             "<th>In</th>\n"
             "<th>Out</th>\n"
             "</tr>\n"
             "</thead>\n",
             CONTENT_TABLE_BOX_CLASS);
      /* Skip first line */
      fgets(line, sizeof(line) - 1, fp);
      /* Read while not end of file */
      while (fgets(line, sizeof(line) - 1, fp)) {
         /* : found? */
         if (strchr(line, ':')) {
            sscanf(line, "%*[ ]%[^:]:%d %*s %*s %*s %*s %*s %*s %*s %d", device, &in, &out);
            /* printf("[%s]<>[%s]\n", device, p->ifa_name); */
            printf("<tr onmouseover=\"this.className='%s';\""
                   " onmouseout=\"this.className='%s';\">\n"
                   "<td>%s</td>\n"
                   "<td>%d MB</td>\n"
                   "<td>%d MB</td>\n"
                   "</tr>\n",
                   CONTENT_TABLE_CLASS_MOUSEOVER,
                   CONTENT_TABLE_CLASS_MOUSEOUT,
                   device,
                   in / 1024 / 1024,
                   out / 1024 / 1024);
         }
      }

      printf("</table>\n");
       /* Close */
      fclose(fp);
   }
}
