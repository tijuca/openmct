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
#include "includes/file.h"
#include "includes/misc.h"
#include "includes/owi.h"
#include "includes/nfs.h"

struct file_data_t nfs_data[] = {
   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "directory",
     NFS_NAME_DIRECTORY,
     NFS_DESCRIPTION_DIRECTORY,
     NULL,
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
     0,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_LIST | FILE_DATA_FLAG_ID
   },

   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "source",
     NFS_NAME_SOURCE,
     NFS_DESCRIPTION_SOURCE, 
     NULL,
     "^[A-Za-z0-9]{4,40}$",
     NULL,
     4,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE | FILE_DATA_FLAG_LIST
   },

   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "source",
     NFS_NAME_OPTIONS,
     NFS_DESCRIPTION_OPTIONS, 
     NULL,
     "^[A-Za-z0-9]{4,40}$",
     NULL,
     4,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE | FILE_DATA_FLAG_LIST
   },

   { 
      0,
      -1,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      0,
      NULL,
      0,
   }
};

/* \fn nfs_main(argc, argv)
 * Show all nfss from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int nfs_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f;

   /* Set correct type */
   f.type = FILE_TYPE_LINE;
   /* Set config settings */
   f.fd = nfs_data;
   /* Set separator */
   f.separator = NFS_SEPARATOR;
   /* Read config into memory */
   file_open(&f, NFS_FILE);

   /* Print header information */
   owi_header(NFS_HEADLINE);

   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print nfs list */
      owi_list(&f);
   } else if (!strcasecmp(command, OWI_BUTTON_DETAIL)) {
      owi_detail_id(&f, variable_get("id"));
   } else if (!strcasecmp(command, OWI_BUTTON_UPDATE)) {
      owi_update_id(&f, variable_get("id"), NFS_FILE_UPDATE, NFS_FILE_ERROR);
   } else if (!strcasecmp(command, OWI_BUTTON_DELETE)) {
      owi_delete_id(&f, variable_get("id"));
   } else if (!strcasecmp(command, OWI_BUTTON_NEW)) {
      owi_new(&f);
   } else if (!strcasecmp(command, OWI_BUTTON_ADD)) {
      owi_add(&f);
   }

   /* Free file */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}
