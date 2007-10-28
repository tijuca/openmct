/* -*- mode: C; c-file-style: "gnu" -*- */
/* rc.c User management
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

/* Define ini configuration tags */
struct file_data_t rc_data[] = {
   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "nfsd",
     RC_NAME_START_NFSD,
     NULL,
     NULL,
     "yes|no",
     "START_NFSD",
     0,
     "yes",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "ftpd",
     RC_NAME_START_FTPD,
     NULL,
     NULL,
     "yes|no",
     "START_FTPD",
     0,
     "yes",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "smb",
     RC_NAME_START_SMBD,
     NULL,
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
     0
   }
};

/* \fn rc_main(argc, argv)
 * Show all rcs from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int rc_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f;

   /* Set correct type */
   f.type = FILE_TYPE_SECTION;
   /* Set config settings */
   f.fd = rc_data;
   /* Set separator */
   f.separator = RC_SEPARATOR;
   /* Read config into memory */
   file_open(&f, RC_FILE);
 
   /* Print header information */
   owi_header(RC_HEADLINE);

   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print rc option list */
      owi_list(&f, NULL);
   } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
      /* Update configuration failed */
      owi_update(&f, RC_FILE_UPDATE, RC_FILE_ERROR);
      /* Just print rc option list */
      owi_list(&f, NULL);
   } 
   /* Free file */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}
