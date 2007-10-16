/* -*- mode: C; c-file-style: "gnu" -*- */
/* user.c User management
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
#include "includes/user.h"

struct file_data_t user_data[] = {
   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "login",
     USER_NAME_LOGIN,
     USER_DESCRIPTION_LOGIN,
     NULL,
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
     0,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_LIST | FILE_DATA_FLAG_ID
   },

   {
     FILE_DATA_TYPE_PASSWORD,
     -1,
     "password",
     USER_NAME_PASSWORD,
     USER_DESCRIPTION_PASSWORD,
     NULL, 
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
     1,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE | FILE_DATA_FLAG_DONTFILL |
     FILE_DATA_FLAG_CRYPT
   },

   { 
     FILE_DATA_TYPE_TEXT,
     -1,
     "gecos",
     USER_NAME_GECOS,
     USER_DESCRIPTION_GECOS, 
     NULL,
     "^[A-Za-z0-9]{4,40}$",
     NULL,
     4,
     NULL,
     FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE | FILE_DATA_FLAG_LIST
   },

   {
      FILE_DATA_TYPE_CHECKBOX,
      -1,
      "shell",
      USER_NAME_SHELL,
      USER_DESCRIPTION_SHELL,
      NULL,
      "yes|no",
      NULL,
      -1,
      NULL,
      FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE
   },

   {
      FILE_DATA_TYPE_INTERNAL,
      -1,
      "shell_internal",
      USER_NAME_SHELL,
      USER_DESCRIPTION_SHELL,
      NULL,
      "/bin/sh|/bin/false",
      NULL,
      6,
      "/bin/false",
      FILE_DATA_FLAG_ADD | FILE_DATA_FLAG_UPDATE
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

/* \fn user_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int user_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header(USER_HEADLINE);

   /* Read file into memory */
   if (file_open(USER_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         user_list();
      /* show user detail */
      } else if (!strcasecmp(command, OWI_BUTTON_DETAIL)) {
         /* Show user detail */
         user_detail(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_UPDATE)) {
	 /* Update user now */
         user_update(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_NEW)) {
	 /* Show form for adding a user */
         user_new(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_ADD)) {
	 /* Add user now */
         user_add(variable_get("id"));
      } else if (!strcasecmp(command, OWI_BUTTON_DELETE)) {
	 /* Delete user now */
         user_delete(variable_get("id"));
      }
      /* Free file */
      file_free();
   } else {
      /* Print error message */
      owi_headline(1, USER_HEADLINE);
      owi_headline(2, USER_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn user_list()
 * Show all users from system
 */
void user_list() {
   /* Index counter */
   int i = 0;

   /* Print outside table */
   owi_outside_open(OWI_LIST);

   /* Print table header */
   owi_table_header(user_data);

   /* Loop through all user entries in passwd file */
   for (i = 0; i < file_line_counter; i++) {
      /* Parse passwd entry */
      char **passwd = NULL;
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      passwd = argument_parse(file_line_get(i), ":");
      /* Display line */
      owi_data_list(user_data, passwd);
      /* Free passwd entry */
      argument_free(passwd);
   }

   /* Print table footer / close internal table / close scroll / print footer / ... */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_NEW);
}

/* \fn user_detail(username)
 * Show one user account
 * \param[in] username user that will be displayed
 */
void user_detail(char *username) {
   /* User found? */
   int user_found = 0;
   /* Index counter */
   int i = 0;

   /* Print info box if variable info is set */
   owi_box_info();

   /* Print error box if variable info is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Loop through all user entries in /etc/passwd */
   for (i = 0; i < file_line_counter; i++) {
      /* Parse passwd entry */
      char **passwd = NULL;
      /* Skip inactive users */
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      /* Parse entry */
      passwd = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(passwd, 0), username)) {
         /* Get details from config line */
	 file_data_detail(user_data, passwd);
	 /* Display details */
         owi_data_detail(user_data);
         /* Set user found to one */
         user_found = 1;
      }
      /* Free passwd entry */
      argument_free(passwd);
   }

   /* No user found? */
   if (!user_found) {
      /* Print information screen */
      owi_headline(2, USER_NOT_FOUND);
   }

   /* Print Submit button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_UPDATE);
}

/* \fn user_update(username)
 * Update an entry in password file
 * \param[in] username username that will be updated
 */
void user_update(char *username) {
   /* Index counter */
   int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Username found? */
      if (!strcmp(argument_get_part(passwd, 0), username)) {
         file_data_update_column(user_data, i, passwd);
      }
      argument_free(passwd);
   }

   if (!strcmp(variable_get("error"), "")) {
      /* Set info */
      variable_set("info", USER_FILE_UPDATE);
      /* Save file */
      file_save(USER_FILE);
      /* Display user list */
      user_list();
   } else {
      /* Set error */
      variable_set("error", USER_FILE_ERROR);
      /* Display user */
      user_detail(username);
   }
}

/* \fn user_add(username)
 * Execute a new user now
 * \param[in] username username that will be updated
 */
void user_add(char *username) {
   /* Index counter */
   int i;
   /* Max uid */
   unsigned int start_uid = 1000;
   /* Error variable */
   char *error = NULL;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Current uid start uid found? */
      if (atoi(argument_get_part(passwd, 2)) == start_uid) {
         /* Increase start uid */
         start_uid++;
	 /* And scan again */
	 i = 0;
      }
      /* New user already in user database */
      if (!strcasecmp(argument_get_part(passwd, 0), 
          variable_ltrim(variable_filter(username, ":")))) {
         /* Set error to already exists */
         error = USER_ALREADY_EXISTS;
      } 
      /* Free passwd entry */
      argument_free(passwd);
      /* Found? */
      if (error) {
         break;
      }
   }

   /* User not found? */
   if (!error) {
      /* Save result in user file */
      file_save(USER_FILE);
      /* Display user list */
      user_list();
   } else {
      variable_set("error", error);
      /* Display user add page with error */
      user_new();
   }
}

/* \fn user_delete(username)
 * Delete an entry from password file
 * \param[in] username username that will be deleted
 */
void user_delete(char *username) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(username, argument_get_part(passwd, 0))) {
         /* Set new passwd line in memory */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free passwd entry */
      argument_free(passwd);
   }

   /* Save result in user file */
   file_save(USER_FILE);

   /* Display user list */
   user_list();
}

/* \fn user_new()
 * Show the add screen for adding a user 
 */
void user_new() {
   /* Print error box if variable error is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open(OWI_DETAIL);

   /* Print all required fields for add request */
   owi_data_detail(user_data);

   /* Print command button */
   owi_outside_close(OWI_DETAIL, OWI_BUTTON_ADD);
}
