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
#include "includes/file.h"
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
   /* System information */
   struct sysinfo si;
   /* Hostname */
   char *hostname = proc_read_line("/bin/hostname -f", 0);
   /* Version */
   char *version = file_read_line("/etc/openmct.release", 0);
   /* OS Version */
   char *osversion = proc_read_line("/bin/uname -a", 0);
   /* Datum */
   char *date = proc_read_line("date", 0);
   /* Uptime */
   char *uptime = proc_read_line("uptime", 0);

   sysinfo(&si);

   printf("<h4>%s</h4>\n"
          "<table class=\"detail\">\n"
          "<thead>\n"
	  "<tr>\n"
	  "<th colspan=\"2\">%s</th>\n"
	  "</tr>\n"
	  "</thead>\n"
	  "<tbody>\n"
	  "<tr>\n"
	  "<td class=\"description\">Name</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">Version</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">OS Version</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">Date</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">Uptime</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">Memory Usage</td>\n"
	  "<td class=\"value\">%ld %%</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"description\">Diskspace Usage</td>\n"
	  "<td class=\"value\">%s</td>\n"
	  "</tr>\n"
          "</tbody>\n"
	  "</table>\n",
	  SYSINFO_HEADLINE,
	  SYSINFO_HEADLINE_BOX,
	  hostname,
	  version,
	  osversion,
	  date,
	  uptime,
	  (si.totalram - si.freeram) / (si.totalram * 100),
	  "0");
}
