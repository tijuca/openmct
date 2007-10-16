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
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/ftp.h"

/* Define ini configuration tags */
struct file_data_t ftp_ini[] = {
   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "port",
     FTP_NAME_LISTEN_PORT,
     NULL,
     NULL,
     "^[0-9]{2,5}$",
     "listen_port",
     0,
     "21",
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "maxclients",
     FTP_NAME_MAX_CLIENTS,
     FTP_DESCRIPTION_MAX_CLIENTS,
     NULL,
     "^[0-9]{1,3}$",
     "max_clients",
     0,
     "5",
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "maxperip",
     FTP_NAME_MAX_PER_IP,
     FTP_DESCRIPTION_MAX_PER_IP,
     NULL,
     "^[0-9]{1,3}$",
     "max_per_ip",
     0,
     "2",
     0
   },

   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "idletimeout",
     FTP_NAME_IDLE_SESSION_TIMEOUT,
     FTP_DESCRIPTION_IDLE_SESSION_TIMEOUT,
     NULL,
     "^[0-9]{1,3}$",
     "idle_session_timeout",
     0,
     "300",
     0
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "anonymous",
     FTP_NAME_ANONYMOUS_ENABLE,
     FTP_DESCRIPTION_ANONYMOUS_ENABLE,
     NULL,
     "yes|no",
     "anonymous_enable",
     0,
     "no",
     0
   },

   { 
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "local",
     FTP_NAME_LOCAL_ENABLE,
     FTP_DESCRIPTION_LOCAL_ENABLE,
     NULL,
     "yes|no",
     "local_enable",
     0,
     "yes",
     0
   },

   {
     FILE_DATA_TYPE_TEXTAREA,
     -1,
     "banner",
     FTP_NAME_BANNER,
     FTP_DESCRIPTION_BANNER,
     NULL,
     "^[0-9A-Za-z .]{0,255}$",
     "ftpd_banner",
     0,
     NULL,
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "createmask",
     FTP_NAME_FILE_OPEN_MODE,
     FTP_DESCRIPTION_FILE_OPEN_MODE,
     NULL,
     "^[0-7]{3}$",
     "file_open_mode",
     0,
     NULL,
     0
   },

   { 
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "fxp",
     FTP_NAME_PASV_PROMISCUOUS,
     FTP_DESCRIPTION_PASV_PROMISCUOUS,
     NULL,
     "yes|no",
     "pasv_promiscuous",
     0,
     "yes",
     0
   },

   { 0,
     -1,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     0,
     NULL,
     0
   }
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
   owi_header(FTP_HEADLINE);

   /* Read file into memory */
   if (file_open(FTP_FILE) != -1) {
      /* Read data */
      file_data_read(ftp_ini, "=");
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print ftp list */
         ftp_list();
      } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
         /* Update configuration failed */
	 ftp_update();
	 /* Reload konfiguration */
	 proc_open(FTP_RESTART);
      } 
      /* Free file */
      file_free();
   } else {
      /* Print error message */
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
   /* Print info box if variable info is set */
   owi_box_info();

   /* Print error box if variable info is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Display all settings in HTML */
   owi_data_detail(ftp_ini);

   /* Print Submit button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_UPDATE);
}

/* \fn ftp_update()
 * Update FTP configuration file
 */
void ftp_update() {
   /* Update ini configuration */
   file_data_update(ftp_ini, "=");

   /* Errors during update? */
   if (!strcmp(variable_get("error"), "")) {
      /* Save result in ftp file */
      file_save(FTP_FILE);

      /* Set info message box */
      variable_set("info", FTP_FILE_UPDATE);
   } else {
      /* Set error message box */
      variable_set("error", FTP_FILE_ERROR);

   }

   /* Display new configuration now*/
   ftp_list();
}
