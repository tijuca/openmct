/* -*- mode: C; c-file-style: "gnu" -*- */
/* smb.c Interface management
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
#include "includes/owi.h"
#include "includes/rc.h"
#include "includes/smb.h"

/* Define ini configuration tags */
struct file_data_t smb_data[] = {
   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "workgroup",
     SMB_NAME_WORKGROUP,
     SMB_DESCRIPTION_WORKGROUP,
     NULL,
     "^[A-Z]{1,40}$",
     "workgroup",
     0,
     "OPENMCT",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "server_string",
     SMB_NAME_SERVER_STRING,
     SMB_DESCRIPTION_SERVER_STRING,
     NULL,
     "^[A-Za-z0-9 ]{1,40}$",
     "server string",
     0,
     "OpenMCT Windows Share Server",
     FILE_DATA_FLAG_UPDATE
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

/* Define ini configuration tags */
struct file_data_t smb_rc_data[] = {
   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "enable_smbd",
     RC_NAME_START_SMBD,
     RC_DESCRIPTION_START_SMBD,
     NULL,
     "yes|no",
     "START_SAMBA",
     0,
     "yes",
     FILE_DATA_FLAG_UPDATE
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
     FILE_DATA_FLAG_UPDATE
   }
};

/* \fn smb_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int smb_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f, f_rc;

   /* Set correct type */
   f.type = FILE_TYPE_SECTION;
   /* Set config settings */
   f.fd = smb_data;
   /* Set separator */
   f.separator = SMB_SEPARATOR;
   /* Read config into memory */
   file_open(&f, SMB_FILE);

   /* Set correct type */
   f_rc.type = FILE_TYPE_SECTION;
   /* Set config settings */
   f_rc.fd = smb_rc_data;
   /* Set separator */
   f_rc.separator = RC_SEPARATOR;
   /* Read config into memory */
   file_open(&f_rc, RC_FILE);
 
   /* Print header information */
   owi_header(SMB_HEADLINE);

   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print user list */
      owi_list(&f_rc, &f, NULL);
   } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
      /* Update settings configuration */
      owi_update(&f_rc, SMB_FILE_UPDATE, SMB_FILE_ERROR);
      /* Update configuration */
      owi_update(&f, SMB_FILE_UPDATE, SMB_FILE_ERROR);
      /* Display configuration */
      owi_list(&f_rc, &f, NULL);
   }

   /* Free file */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}
