/* -*- mode: C; c-file-style: "gnu" -*- */
/* ftp.c User management
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
#include <unistd.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/file.h"
#include "includes/ftp.h"

/* Define ini configuration tags */
struct file_ini_t ftp_ini[] = {
   { FTP_INI_LISTEN, "listen", "YES|NO", "YES", NULL, 0 },
   { FTP_INI_ANONYMOUS_ENABLE, "anonymous_enable", "YES|NO", "NO", NULL, 0 },
   { FTP_INI_LOCAL_ENABLE, "local_enable", "YES|NO", "YES", NULL, 0 },
   { FTP_INI_WRITE_ENABLE, "write_enable", "YES|NO", "YES", NULL, 0 },
   { FTP_INI_LOCAL_UMASK, "local_umask", NULL, "022", NULL, 0 },
   { FTP_INI_DIRMESSAGE_ENABLED, "dirmessage_enable", "YES|NO", "YES", NULL, 0 },
   { FTP_INI_XFERLOG_ENABLE, "xferlog_enable", "YES|NO", "NO", NULL, 0 },
   { FTP_INI_CONNECT_FROM_PORT_20, "connect_from_port_20", "YES|NO", "NO", NULL, 0 },
   { FTP_INI_SECURE_CHROOT_DIR, "secure_chroot_dir", NULL, "/tmp", NULL, 0 },
   { FTP_INI_FTPD_BANNER, "ftpd_banner", NULL, "Welcome on the OpenMCT Release!", NULL, 0},
   { NULL, NULL, NULL, NULL, NULL, 0 }
};

/* \fn ftp_main(argc, argv)
 * Show all ftps from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int ftp_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header("FTP Management");

   /* Read file into memory */
   if (file_open(FTP_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print ftp list */
         ftp_list();
      } else if (!strcmp(command, "update")) {
         /* Update configuration failed */
	 ftp_update();
	 /* Reload konfiguration */
	 proc_open("killall -HUP vsftpd");
      } 
      /* Free file */
      file_free(FTP_FILE);
   } else {
      /* Print error message */
      owi_headline(1, FTP_HEADLINE);
      owi_headline(2, FTP_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn ftp_list()
 * Show all ftps from system
 */
void ftp_list() {
   /* Index counter */
   int i = 0;
   /* Listen line */
   char **ini_listen = NULL;
   /* Index counter */
   int j = 0;

   /* Print external table for design */
   printf("<table class=\"%s\">\n"
          "<tr>\n"
          "<td>\n"
          "<h1>%s</h1>\n"
          "<br />%s<br /><br />\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"command\" value=\"update\" />\n"
          "<table class=\"%s\" width=\"100%%\" cellspacing=\"0\" cellpading=\"0\">\n"
	  "<thead>\n"
	  "<tr>\n"
	  "<th colspan=\"2\">FTP Konfiguration</th>\n"
	  "</tr>\n"
	  "</thead>\n",
          CONTENT_TABLE_CLASS,
          FTP_HEADLINE,
          FTP_DETAIL,
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_BOX_CLASS);

   /* Loop through config file */
   for (i = 0;  i < file_line_counter; i++) {
      /* Parse line */
      char **ini = argument_parse(file_line_get(i), "=");

      /* Parse through all available commands */
      for (j = 0; ftp_ini[j].variable != NULL; j++) { 
         if (!strcasecmp(argument_get_part(ini, 0), ftp_ini[j].variable)) {
            ftp_ini[j].current = ini;
	 }
      }
   }

   /* Loop through all availbe config variables */
   for (i = 0; ftp_ini[i].variable != NULL; i++) {
      printf("<tr>\n"
             "<td width=\"250\">%s</td>\n"
	     "<td>", ftp_ini[i].description);
      /* valid set? */
      if (ftp_ini[i].valid) {
         /* Valid values */
	 char **valid_values = argument_parse(ftp_ini[i].valid, "|");

         printf("<select name=\"%s\">\n", ftp_ini[i].variable);
         /* Loop through values */
	 for (j = 0; valid_values[j] != NULL; j++) {
	    /* Current value */
            int current_value = strcmp(ftp_ini[i].current[1], valid_values[j]);
	    /* Print on stdout */
            printf("<option value=\"%s\" %s>%s</option>\n",
	           valid_values[j], current_value == 0 ? " selected" : "", valid_values[j]);
	 }
	 printf("</select>\n");
	 /* Free valid values */
	 argument_free(valid_values);
      } else {
         printf("<input type=\"text\" name=\"%s\" value=\"%s\" />\n",
	        ftp_ini[i].variable, ftp_ini[i].current ? ftp_ini[i].current[1] : "");
      }
      printf("</td>\n"
             "</tr>\n");
   }

   /* Print Submit button */
   printf("</table>\n"
          "<table width=\"100%%\">\n"
	  "<tr>\n"
	  "<td align=\"right\"><input type=\"submit\" value=\"%s\" /></td>\n"
	  "</tr>\n"
          "</table>\n"
          "</form>\n",
	  FTP_BUTTON_UPDATE);

   /* Listen directive found? */	  
   if (ini_listen) {
      /* Free data */
      argument_free(ini_listen);
   }
}

/* \fn ftp_update()
 * Update FTP configuration file
 */
void ftp_update() {
   /* Index counter */
   int i = 0;
   /* Index counter */
   int j = 0;

   /* Loop through config file */
   for (i = 0; i < file_line_counter; i++) {
      /* Parse line */
      char **ini = argument_parse(file_line_get(i), "=");

      /* Parse through all available commands */
      for (j = 0; ftp_ini[j].variable != NULL; j++) {
         /* Entry found? */
         if (!strcasecmp(argument_get_part(ini, 0), ftp_ini[j].variable)) {
            /* Write new config line */
            file_line_action(FILE_LINE_SET, i,
	                     "%*s=%s",
			     file_get_pad(file_line_get(i), ftp_ini[j].variable),
			     ftp_ini[j].variable,
			     variable_get(ftp_ini[j].variable));
            /* Mark entry as found */
            ftp_ini[j].found = 1;
         }
      }
   }

   /* Loop trough variables */
   for (j = 0; ftp_ini[j].variable != NULL; j++) {
      /* Not found? */
      if (!ftp_ini[j].found) {
         /* Append to config file */
	 file_line_action(FILE_LINE_ADD, file_line_counter,
	                  "%s=%s",
			  ftp_ini[j].variable,
			  variable_get(ftp_ini[j].variable));
      }
   }

   /* Save result in ftp file */
   file_save(FTP_FILE);

   /* Display new configuration now*/
   ftp_list();
}
