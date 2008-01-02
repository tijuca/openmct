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
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/user.h"

struct data_t user_data[] = {
   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_LIST | DATA_FLAG_ID,
     USER_NAME_LOGIN,
     USER_DESCRIPTION_LOGIN,
     "login",
     "login",
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
   },

   {
     DATA_TYPE_PASSWORD,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_DONT_FILL | DATA_FLAG_CRYPT,
     USER_NAME_PASSWORD,
     USER_DESCRIPTION_PASSWORD,
     "password",
     "password",
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
   },

   {
     DATA_TYPE_INTERNAL,
     0,
     NULL,
     NULL,
     "uid",
     "uid",
     "^[0-9]{1,5}$",
     USER_UID_STANDARD
   },

   {
     DATA_TYPE_INTERNAL,
     0,
     NULL,
     NULL,
     "gid",
     "gid",
     "^[0-9]{1,5}$",
     USER_GID_STANDARD
   },

   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     USER_NAME_GECOS,
     USER_DESCRIPTION_GECOS, 
     "gecos",
     "gecos",
     "^[A-Za-z0-9]{0,40}$",
     NULL,
   },

   {
     DATA_TYPE_INTERNAL,
     0,
     NULL,
     NULL,
     "homedirectory",
     "homedirectory",
     "^[A-Za-z0-9/]{4,40}$",
     NULL,
   },

   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE,
     USER_NAME_SHELL,
     USER_DESCRIPTION_SHELL,
     "shell",
     "shell",
     "/bin/sh|/bin/false",
     "/bin/false",
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

/* \fn user_main(owi)
 * Show all users from system
 * \param[in] owi handler
 * \return zero on sucess
 */
int user_main(struct owi_t *owi) {
   /* Set separator */
   owi->file->separator = string_copy_value(USER_SEPARATOR);
   /* Set filename */
   owi->file->name = string_copy_value(USER_FILE);
   /* Set owi properties for display */
   owi->headline = string_copy_value(USER_HEADLINE);
   owi->data = user_data;
   owi->flags = OWI_FLAG_ACTION | OWI_FLAG_ACTION_DETAIL |
                OWI_FLAG_ACTION_DELETE | OWI_FLAG_ACTION_UPDATE |
                OWI_FLAG_ROW;
   owi->callback = user_callback;
   /* Start main */
   owi_main(owi);
   /* Return success */
   return 0;
}

/* \fn user_callback(owi)
 * Callback function for owi actions
 * \param[in] owi OWI structure
 * \param[in] command module command
 */
struct string_t *user_callback(struct owi_t *owi, struct string_t *command) {
   return NULL;
}
