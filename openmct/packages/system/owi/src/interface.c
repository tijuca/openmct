/* -*- mode: C; c-file-style: "gnu" -*- */
/* lan.c Interface management
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

extern int file_section_start;
extern int file_section_stop;

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
     FILE_DATA_FLAG_SKIP_EMPTY
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
     FILE_DATA_FLAG_SKIP_EMPTY
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
     FILE_DATA_FLAG_SKIP_EMPTY
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
     FILE_DATA_FLAG_SKIP_EMPTY
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
 
   /* Print header information */
   owi_header(INTERFACE_LAN_HEADLINE);

   /* Read file into memory */
   if (file_open(INTERFACE_FILE) != -1) {
      /* Get section */
      interface_section_get(INTERFACE_LAN);

      /* Read data from secion */
      file_data_read(interface_data, " ");
   
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         interface_list(INTERFACE_LAN);
      } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
         interface_update(INTERFACE_LAN);
      }
      /* Free file */
      file_free();
   } else {
      /* Print error message */
      owi_headline(1, INTERFACE_LAN_HEADLINE);
      owi_headline(2, INTERFACE_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn interface_list(interface)
 * Show interface
 * \param[in] interface read interface from file
 */
void interface_list(char *interface) {
   /* Print info box if variable info is set */
   owi_box_info();

   /* Print error box if variable info is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Display all settings in HTML */
   owi_data_detail(interface_data);

   /* Print Submit button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_UPDATE);
}

void interface_section_get(char *interface) {
   /* Index counte r*/
   int i = 0;

   /* Get section for current interface */
   i = 0;
   /* Loop through all interfaces in interfaces file */
   while (i <= file_line_counter) {
      /* Parse entry */
      char **entry = NULL;
      /* Pseudo line not reached? */
      if (i < file_line_counter) {
         /* Get and parse line */
         entry = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      }
      /* Auto line for LAN interface? */
      if (entry && entry[0] && !strcasecmp(entry[0], "auto")) {
         if (!strcmp(entry[1], interface)) {
            file_section_start = i;
         } else if (file_section_start != -1 &&
	            file_section_stop == -1) {
            file_section_stop = i;
         }
      }
      /* End of interfaces file reached? */
      if (i == file_line_counter) {
         /* End not found? */
         if (file_section_start >= 0 && file_section_stop == -1) {
            file_section_stop = file_line_counter;
         }
      }
      /* Increase counter */
      i++;
      /* Free argument */
      free(entry);
   }
}

void interface_update(char *interface) {
   /* Update ini configuration */
   file_data_update(interface_data, " ");

   /* Errors during update? */
   if (!strcmp(variable_get("error"), "")) {
      /* Save result in ftp file */
      file_save(INTERFACE_FILE);

      /* Set info message box */
      variable_set("info", INTERFACE_FILE_UPDATE);
   } else {
      /* Set error message box */
      variable_set("error", INTERFACE_FILE_ERROR);
   }

   /* Display new configuration now*/
   interface_list(interface);
}
