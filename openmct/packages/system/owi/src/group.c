/* -*- mode: C; c-file-style: "gnu" -*- */
/* group.c Group management
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
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/file.h"
#include "includes/misc.h"
#include "includes/owi.h"
#include "includes/group.h"

struct file_data_t group_data[] = {
   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "login",
     GROUP_NAME_LOGIN,
     GROUP_DESCRIPTION_LOGIN,
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
     "members",
     GROUP_NAME_MEMBERS,
     GROUP_DESCRIPTION_MEMBERS, 
     NULL,
     "^[A-Za-z0-9,]{0,40}$",
     NULL,
     3,
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

/* \fn group_main(argc, argv)
 * Show all groups from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int group_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header(GROUP_HEADLINE);

   /* Read file into memory */
   if (file_open(GROUP_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print group list */
         group_list();
      /* show group detail */
      } else if (!strcasecmp(command, OWI_BUTTON_DETAIL)) {
         /* Show group detail */
         group_detail(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_UPDATE)) {
	 /* Update group now */
         group_update(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_NEW)) {
	 /* Show form for adding a group */
         group_new(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_ADD)) {
	 /* Add group now */
         group_add(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_DELETE)) {
	 /* Delete group now */
         group_delete(variable_get("id"));
      }
      /* Free file */
      file_free();
   } else {
      /* Print error message */
      owi_headline(1, GROUP_HEADLINE);
      owi_headline(2, GROUP_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn group_list()
 * Show all groups from system
 */
void group_list() {
   /* Index counter */
   int i = 0;

   /* Print outside table */
   owi_outside_open(OWI_LIST);

   /* Print table header */
   owi_table_header(group_data);

   /* Loop through all group entries in group file */
   for (i = 0; i < file_line_counter; i++) {
      /* Parse group entry */
      char **group = NULL;
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      group = argument_parse(file_line_get(i), ":");
      /* Display line */
      owi_data_list(group_data, group);
      /* Free group entry */
      argument_free(group);
   }

   /* Print table footer / close internal table / close scroll / print footer / ... */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_NEW);
}

/* \fn group_detail(groupname)
 * Show one group account
 * \param[in] groupname group that will be displayed
 */
void group_detail(char *groupname) {
   /* Group found? */
   int group_found = 0;
   /* Index counter */
   int i = 0;

   /* Print info box if variable info is set */
   owi_box_info();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Loop through all group entries in group file */

   for (i = 0; i < file_line_counter; i++) {
      /* Parse group entry */
      char **group = NULL;
      /* Skip inactive groups */
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      /* Parse entry */
      group = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(group, 0), groupname)) {
         /* Get details from config line */
	 file_data_detail(group_data, group);
	 /* Display details */
         owi_data_detail(group_data);
         /* Set group found to one */
         group_found = 1;
      }
      /* Free group entry */
      argument_free(group);
   }

   /* No group found? */
   if (!group_found) {
      /* Print information screen */
      owi_headline(2, GROUP_NOT_FOUND);
   }

   /* Print Submit button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_UPDATE);
}

/* \fn group_update(groupname)
 * Update an entry in password file
 * \param[in] groupname groupname that will be updated
 */
void group_update(char *groupname) {
   /* Index counter */
   int i;

   /* Loop through group entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get group entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Groupname found? */
      if (!strcmp(argument_get_part(group, 0), groupname)) {
         file_data_update_column(group_data, i, group);
      }
      argument_free(group);
   }

   if (!strcmp(variable_get("error"), "")) {
      /* Set info */
      variable_set("info", GROUP_FILE_UPDATE);
      /* Save file */
      file_save(GROUP_FILE);
      /* Display group list */
      group_list();
   } else {
      variable_set("error", GROUP_FILE_ERROR);
      group_detail(groupname);
   }
}

/* \fn group_add(groupname)
 * Execute a new group now
 * \param[in] groupname groupname that will be updated
 */
void group_add(char *groupname) {
   /* Index counter */
   int i;
   /* Max uid */
   unsigned int start_uid = 1000;
   /* Error variable */
   char *error = NULL;

   /* Loop through group entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get group entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Current uid start uid found? */
      if (atoi(argument_get_part(group, 2)) == start_uid) {
         /* Increase start uid */
         start_uid++;
	 /* And scan again */
	 i = 0;
      }
      /* New group already in group database */
      if (!strcasecmp(argument_get_part(group, 0), 
          variable_ltrim(variable_filter(groupname, ":")))) {
         /* Set error to already exists */
         error = GROUP_ALREADY_EXISTS;
      } 
      /* Free group entry */
      argument_free(group);
      /* Found? */
      if (error) {
         break;
      }
   }

   /* Group not found? */
   if (!error) {
      /* Save result in group file */
      file_save(GROUP_FILE);
      /* Display group list */
      group_list();
   } else {
      variable_set("error", error);
      /* Display group add page with error */
      group_new();
   }
}

/* \fn group_delete(groupname)
 * Delete an entry from password file
 * \param[in] groupname groupname that will be deleted
 */
void group_delete(char *groupname) {
   /* Index counter */
   unsigned int i;

   /* Loop through group entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get group entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(groupname, argument_get_part(group, 0))) {
         /* Set new group line in memory */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free group entry */
      argument_free(group);
   }

   /* Save result in group file */
   file_save(GROUP_FILE);

   /* Display group list */
   group_list();
}

/* \fn group_new()
 * Show the add screen for adding a group 
 */
void group_new() {
   /* Print error box if variable error is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Print all required fields for add request */
   owi_data_detail(group_data);

   /* Print command button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_ADD);
}
