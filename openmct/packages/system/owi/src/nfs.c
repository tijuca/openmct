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
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/variable.h"
#include "includes/data.h"
#include "includes/file.h"
#include "includes/misc.h"
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
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     NFS_NAME_SOURCE,
     NFS_DESCRIPTION_SOURCE, 
     "source",
     "source",
     "^[A-Za-z0-9]{4,40}$",
     NULL,
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     NFS_NAME_SOURCE,
     NFS_DESCRIPTION_SOURCE, 
     "source",
     "source",
     "^[A-Za-z0-9]{4,40}$",
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

/* \fn nfs_main(argc, argv)
 * Show all nfss from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int nfs_main(int argc, char **argv) {
   /* File structure */
   struct file_t file;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = NFS_SEPARATOR;
   /* Read config into memory */
   file_open(&file, NFS_FILE);

   /* Set owi properties for display */
   owi.headline = NFS_HEADLINE;
   owi.file = &file;
   owi.file_init = NULL;
   owi.data = nfs_data;
   owi.data_init = NULL;
   owi.button = NULL;
   owi.flags = OWI_FLAG_ACTION | OWI_FLAG_ROW;

   /* Start main */
   owi_main(&owi);

   /* Free data file */
   file_free(&file);

   /* Return success */
   return 0;
}
