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
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/interface.h"

/* Define ini configuration tags */
struct data_t interface_data[] = {
   {
     DATA_TYPE_TEXT,
     DATA_FLAG_SKIP_EMPTY | DATA_FLAG_UPDATE,
     INTERFACE_NAME_ADDRESS,
     INTERFACE_DESCRIPTION_ADDRESS,
     "address",
     "address",
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "192.168.0.254"
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_SKIP_EMPTY | DATA_FLAG_UPDATE,
     INTERFACE_NAME_NETMASK,
     INTERFACE_DESCRIPTION_NETMASK,
     "netmask",
     "netmask",
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "255.255.255.0"
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_SKIP_EMPTY | DATA_FLAG_UPDATE,
     INTERFACE_NAME_BROADCAST,
     INTERFACE_DESCRIPTION_BROADCAST,
     "broadcast",
     "broadcast",
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "192.168.0.255"
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_SKIP_EMPTY | DATA_FLAG_UPDATE,
     INTERFACE_NAME_GATEWAY,
     INTERFACE_DESCRIPTION_GATEWAY,
     "gateway",
     "gateway",
     "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$",
     "192.168.0.1"
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

/* \fn lan_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int lan_main(int argc, char **argv) {
   /* File structure */
   struct file_t file;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = INTERFACE_SEPARATOR;
   /* Read config into memory */
   file_open(&file, INTERFACE_FILE);

   /* Set owi properties for display */
   owi.headline = INTERFACE_LAN_HEADLINE;
   owi.file = &file;
   owi.file_init = NULL;
   owi.data = interface_data;
   owi.data_init = NULL;
   owi.button = NULL;
   owi.flags = OWI_FLAG_CONFIG;

   /* Start main */
   owi_main(&owi);

   /* Free data file */
   file_free(&file);

   /* Return success */
   return 0;
}
