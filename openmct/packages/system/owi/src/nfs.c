/* -*- mode: C; c-file-style: "gnu" -*- */
/* nfs.c NFS management
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
#include "includes/nfs.h"

struct data_t nfs_data[] = {
   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_LIST | DATA_FLAG_ID,
     NFS_NAME_DIRECTORY,
     NFS_DESCRIPTION_DIRECTORY,
     "directory",
     "directory",
     "^[A-Za-z0-9_/]{3,8}$",
     NULL,
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     NFS_NAME_SOURCE,
     NFS_DESCRIPTION_SOURCE, 
     "source",
     "source",
     "^[A-Za-z0-9\\.]{4,40}$",
     NULL,
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     NFS_NAME_OPTIONS,
     NFS_DESCRIPTION_SOURCE, 
     "options",
     "options",
     "^[A-Za-z0-9\\.]{4,40}$",
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
struct data_t nfs_rc_data[] = {
   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     RC_NAME_START_NFSD,
     RC_DESCRIPTION_START_NFSD,
     "enable_nfs",
     "START_NFS",
     "yes|no",
     "no",
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

/* \fn nfs_main(owi)
 * Show all nfss from system
 * \param[in] owi handler
 * \return zero on sucess
 */
int nfs_main(struct owi_t *owi) {
   /* Set separator */
   owi->file->separator = string_copy_value(NFS_SEPARATOR);
   /* Skip comment lines */
   owi->file->flags = FILE_FLAG_SKIP_COMMENT;
   /* Set filename */
   owi->file->name = string_copy_value(NFS_FILE);
   /* Set owi properties for display */
   owi->headline = string_copy_value(NFS_HEADLINE);
   owi->data = nfs_data;
   owi->flags = OWI_FLAG_ACTION | OWI_FLAG_ACTION_DELETE |
                OWI_FLAG_ACTION_DETAIL | OWI_FLAG_ACTION_UPDATE |
	        OWI_FLAG_ROW;
   owi->callback = nfs_callback;
   /* Start main */
   owi_main(owi);
   /* Return success */
   return 0;
}

/* \fn nfs_callback(owi, action)
 * Callback function for owi actions
 * \param[in] owi OWI structure
 * \param[in] action action that will be executed after no errors
 */
struct string_t *nfs_callback(struct owi_t *owi, struct string_t *command) {
   return NULL;
}
