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
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/rc.h"
#include "includes/smb.h"

/* Define ini configuration tags */
struct data_t smb_data[] = {
   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     SMB_NAME_WORKGROUP,
     SMB_DESCRIPTION_WORKGROUP,
     "workgroup",
     "workgroup",
     "^[A-Z]{1,40}$",
     "workgroup"
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     SMB_NAME_SERVER_STRING,
     SMB_DESCRIPTION_SERVER_STRING,
     "server_string",
     "server_string",
     "^[A-Za-z0-9 ]{1,40}$",
     "OpenMCT Windows Share Server"
   },

   { 
     0,
     0,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL
   }
};

/* Define ini configuration tags */
struct data_t smb_rc_data[] = {
   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     RC_NAME_START_SMBD,
     RC_DESCRIPTION_START_SMBD,
     "enable_smbd",
     "START_SAMBA",
     "yes|no",
     "no"
   },

   {
     0,
     0,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL
   }
};

/* \fn smb_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int smb_main(int argc, char **argv) {
   /* File structure */
   struct file_t file, file_rc;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = SMB_SEPARATOR;
   /* Read config into memory */
   file_open(&file, SMB_FILE);

   /* Set separator */
   file_rc.separator = RC_SEPARATOR;
   /* Read config into memory */
   file_open(&file_rc, RC_FILE);

   /* Set owi properties for display */
   owi.headline = SMB_HEADLINE;
   owi.file = &file;
   owi.file_init = &file_rc;
   owi.data = smb_data;
   owi.data_init = smb_rc_data;
   owi.button = NULL;
   owi.flags = OWI_FLAG_CONFIG | OWI_FLAG_ACTION_UPDATE;

   /* Start main */
   owi_main(&owi);

   /* Free rc data file */
   file_free(&file_rc);
   /* Free data file */
   file_free(&file);

   /* Return success */
   return 0;
}
