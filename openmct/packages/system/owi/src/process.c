/* -*- mode: C; c-file-style: "gnu" -*- */
/* process.c Process information
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
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/process.h"

struct data_t process_data[] = {
   {
     DATA_TYPE_TEXT,
     DATA_FLAG_LIST | DATA_FLAG_ID,
     "Process ID",
     "Process owner",
     "pid",
     "pid",
     NULL,
     NULL
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_LIST,
     "User",
     "Process owner",
     "user",
     "user",
     NULL,
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     0,
     "VSZ",
     "VSZ",
     "VSZ",
     "VSZ",
     NULL,
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     0,
     "stat",
     "Stat",
     "Stat",
     "stat",
     NULL,
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_LIST,
     "Process",
     "Process name",
     "login",
     "login",
     NULL,
     NULL
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


/* \fn process_main(owi)
 * Process information
 * \param[in] owi handler
 * \return zero on sucess
 */
int process_main(struct owi_t *owi) {
   /* Set separator */
   owi->file->separator = string_copy_value(PROCESS_SEPARATOR);
   /* Set filename */
   owi->file->name = string_copy_value(PROCESS_FILE);
   /* Set owi properties for display */
   owi->headline = string_copy_value(PROCESS_HEADLINE);
   owi->data = process_data;
   owi->flags = OWI_FLAG_ACTION | OWI_FLAG_ACTION_KILL |
                OWI_FLAG_ROW | OWI_FLAG_HIDE_NEW;

   /* Execute command which creates input data in PROCESS_FILE */
   system (PROCESS_COMMAND);
  
   /* Start main */
   owi_main(owi);
  
   /* Kill file created through system() call. */
   /* TODO This is not working */
   /*remove (string_value(owi->file->name));*/

   /* Return success */
   return 0;
}
