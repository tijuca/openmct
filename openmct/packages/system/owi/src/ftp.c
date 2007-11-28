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
#include "includes/variable.h"
#include "includes/file.h"
#include "includes/data.h"
#include "includes/owi.h"
#include "includes/rc.h"
#include "includes/ftp.h"

/* Define ini configuration tags */
struct data_t ftp_data[] = {
   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     FTP_NAME_LISTEN_PORT,
     NULL,
     "port",
     "listen_port",
     "^[0-9]{2,5}$",
     "21",
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     FTP_NAME_MAX_CLIENTS,
     FTP_DESCRIPTION_MAX_CLIENTS,
     "maxclients",
     "max_clients",
     "^[0-9]{1,3}$",
     "5",
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     FTP_NAME_MAX_PER_IP,
     FTP_DESCRIPTION_MAX_PER_IP,
     "maxperip",
     "max_per_ip",
     "^[0-9]{1,3}$",
     "2",
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     FTP_NAME_IDLE_SESSION_TIMEOUT,
     FTP_DESCRIPTION_IDLE_SESSION_TIMEOUT,
     "idletimeout",
     "idle_session_timeout",
     "^[0-9]{1,3}$",
     "300",
   },

   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     FTP_NAME_ANONYMOUS_ENABLE,
     FTP_DESCRIPTION_ANONYMOUS_ENABLE,
     "anonymous",
     "anonymous_enable",
     "yes|no",
     "no",
   },

   { 
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     FTP_NAME_LOCAL_ENABLE,
     FTP_DESCRIPTION_LOCAL_ENABLE,
     "local",
     "local_enable",
     "yes|no",
     "yes",
   },

   {
     DATA_TYPE_TEXTAREA,
     DATA_FLAG_UPDATE,
     FTP_NAME_BANNER,
     FTP_DESCRIPTION_BANNER,
     "banner",
     "ftpd_banner",
     "^[0-9A-Za-z .]{0,255}$",
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     FTP_NAME_FILE_OPEN_MODE,
     FTP_DESCRIPTION_FILE_OPEN_MODE,
     "createmask",
     "file_open_mode",
     "^[0-7]{3}$",
     NULL,
   },

   { 
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     FTP_NAME_PASV_PROMISCUOUS,
     FTP_DESCRIPTION_PASV_PROMISCUOUS,
     "fxp",
     "pasv_promiscuous",
     "yes|no",
     NULL,
   },

   { 
     0,
     0,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
   }
};

/* Define ini configuration tags */
struct data_t ftp_rc_data[] = {
   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     RC_NAME_START_FTPD,
     RC_DESCRIPTION_START_FTPD,
     "enable_ftpd",
     "START_FTPD",
     "yes|no",
     "yes",
   },

   {
     0,
     0,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
   }
};

/* \fn ftp_main(argc, argv)
 * Show all ftps from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int ftp_main(int argc, char **argv) {
   /* File structure */
   struct file_t file, file_rc;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = FTP_SEPARATOR;
   /* Read config into memory */
   file_open(&file, FTP_FILE);

   /* Set separator */
   file_rc.separator = RC_SEPARATOR;
   /* Read config into memory */
   file_open(&file_rc, RC_FILE);

   /* Set owi properties for display */
   owi.headline = FTP_HEADLINE;
   owi.file = &file;
   owi.file_init = &file_rc;
   owi.data = ftp_data;
   owi.data_init = ftp_rc_data;
   owi.button = NULL;
   owi.flags = OWI_FLAG_CONFIG;

   /* Start main */
   owi_main(&owi);

   /* Free rc data file */
   file_free(&file_rc);
   /* Free data file */
   file_free(&file);

   /* Return success */
   return 0;
}
