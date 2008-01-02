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
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/group.h"

struct data_t group_data[] = {
   { 
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_LIST | DATA_FLAG_ID,
     GROUP_NAME_LOGIN,
     GROUP_DESCRIPTION_LOGIN,
     "login",
     "login",
     "^[A-Za-z0-9_]{3,8}$",
     NULL,
   },

   {
     DATA_TYPE_INTERNAL,
     0,
     NULL,
     NULL,
     NULL,
     "password",
     NULL,
     GROUP_PASSWORD_STANDARD
   },

   {
     DATA_TYPE_INTERNAL,
     0,
     NULL,
     NULL,
     NULL,
     "gid",
     "^[0-9]{1,5}$",
     GROUP_GID_STANDARD,
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_ADD | DATA_FLAG_UPDATE | DATA_FLAG_LIST,
     GROUP_NAME_MEMBERS,
     GROUP_DESCRIPTION_MEMBERS, 
     "members",
     "members",
     "^[A-Za-z0-9,]{4,40}$",
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
      NULL
   }
};

/* \fn group_main(owi)
 * Show all groups from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int group_main(struct owi_t *owi) {
   /* Set separator */
   owi->file->separator = string_copy_value(GROUP_SEPARATOR);
   /* Set filename */
   owi->file->name = string_copy_value(GROUP_FILE);
   /* Set owi properties for display */
   owi->headline = string_copy_value(GROUP_HEADLINE);
   owi->data = group_data;
   owi->flags = OWI_FLAG_ACTION | OWI_FLAG_ACTION_DETAIL |
                OWI_FLAG_ACTION_DELETE | OWI_FLAG_ACTION_UPDATE |
	        OWI_FLAG_ROW;
   /* Start main */
   owi_main(owi);
   /* Return success */
   return 0;
}

/* \fn group_callback(owi, action)
 * Callback function for owi actions
 * \param[in] owi OWI structure
 * \param[in] action action that will be executed after no errors
 */
struct string_t *group_callback(struct owi_t *owi, struct string_t *command) {
   return NULL;
}
