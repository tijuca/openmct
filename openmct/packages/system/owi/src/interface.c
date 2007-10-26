/* -*- mode: C; c-file-style: "gnu" -*- */
/* interface.c Interface management
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
#include "includes/interface.h"

/* Define ini configuration tags */
struct file_data_t interface_data[] = {
   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "address",
     INTERFACE_NAME_ADDRESS,
     INTERFACE_DESCRIPTION_ADDRESS,
     NULL,
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "address",
     0,
     "192.168.0.254",
     FILE_DATA_FLAG_SKIP_EMPTY | FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "netmask",
     INTERFACE_NAME_NETMASK,
     INTERFACE_DESCRIPTION_NETMASK,
     NULL,
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "netmask",
     0,
     "255.255.255.0",
     FILE_DATA_FLAG_SKIP_EMPTY | FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "broadcast",
     INTERFACE_NAME_BROADCAST,
     INTERFACE_DESCRIPTION_BROADCAST,
     NULL,
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "broadcast",
     0,
     "192.168.0.255",
     FILE_DATA_FLAG_SKIP_EMPTY | FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "gateway",
     INTERFACE_NAME_GATEWAY,
     INTERFACE_DESCRIPTION_GATEWAY,
     NULL,
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "gateway",
     0,
     "192.168.0.1",
     FILE_DATA_FLAG_SKIP_EMPTY | FILE_DATA_FLAG_UPDATE
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

/* \fn lan_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int lan_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f;

   /* Set correct type */
   f.type = FILE_TYPE_SECTION;
   /* Set config settings */
   f.fd = interface_data;
   /* Set separator */
   f.separator = INTERFACE_SEPARATOR;
   /* Read config into memory */
   file_open(&f, INTERFACE_FILE);
 
   /* Print header information */
   owi_header(INTERFACE_LAN_HEADLINE);

   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print user list */
      owi_detail(&f);
   } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
      /* Update configuration failed */
      owi_update(&f, INTERFACE_FILE_UPDATE, INTERFACE_FILE_ERROR);
      /* Reload konfiguration */
      // proc_open(INTERFACE_RESTART);
      owi_detail(&f);
   }

   /* Free file */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}
