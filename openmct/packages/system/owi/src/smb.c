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
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
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

/* \fn smb_main(owi)
 * Show all users from system
 * \param[in] owi handler
 * \return zero on sucess
 */
int smb_main(struct owi_t *owi) {
   /* Set separator */
   owi->file->separator = string_copy_value(SMB_SEPARATOR);
   /* Set filename */
   owi->file->name = string_copy_value(SMB_FILE);
   /* Set separator */
   owi->file_init->separator = string_copy_value(RC_SEPARATOR);
   /* Set filename */
   owi->file_init->name = string_copy_value(RC_FILE);
   /* Set owi properties for display */
   owi->headline = string_copy_value(SMB_HEADLINE);
   owi->data = smb_data;
   owi->data_init = smb_rc_data;
   owi->flags = OWI_FLAG_CONFIG | OWI_FLAG_ACTION_UPDATE;
   owi->callback = NULL;
   /* Start main */
   owi_main(owi);
   /* Return success */
   return 0;
}
