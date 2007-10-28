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
#include "includes/argument.h"
#include "includes/file.h"
#include "includes/language.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/process.h"

struct file_data_t process_data[] = {
   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "user",
     "User",
     "Process owner",
     NULL,
     NULL,
     NULL,
     0,
     NULL,
     FILE_DATA_FLAG_LIST 
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "pid",
     "Process ID",
     "Process owner",
     NULL,
     NULL,
     NULL,
     1,
     NULL,
     FILE_DATA_FLAG_LIST | FILE_DATA_FLAG_ID
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "cpu",
     "CPU",
     "CPU usage",
     NULL,
     NULL,
     NULL,
     2,
     NULL,
     FILE_DATA_FLAG_LIST
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "mem",
     "MEM",
     "Memory usage",
     NULL,
     NULL,
     NULL,
     3,
     NULL,
     FILE_DATA_FLAG_LIST
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "VSZ",
     "VSZ",
     "VSZ",
     NULL,
     NULL,
     NULL,
     4,
     NULL,
     0,
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "RSS",
     "RSS",
     "RSS",
     NULL,
     NULL,
     NULL,
     5,
     NULL,
     0,
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "TTY",
     "TTY",
     "TTY",
     NULL,
     NULL,
     NULL,
     6,
     NULL,
     0 
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "stat",
     "Stat",
     "Stat",
     NULL,
     NULL,
     NULL,
     7,
     NULL,
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "start",
     "Start",
     "Start",
     NULL,
     NULL,
     NULL,
     8,
     NULL,
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "Time",
     "Time",
     "Time",
     NULL,
     NULL,
     NULL,
     9,
     NULL,
     0
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "login",
     "Process",
     "Process name",
     NULL,
     NULL,
     NULL,
     10,
     NULL,
     FILE_DATA_FLAG_LIST
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

/* \fn process_main(argc, argv)
 * Process information
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int process_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f;

   /* Set correct type */
   f.type = FILE_TYPE_LINE;
   /* Set config settings */
   f.fd = process_data;
   /* Set separator */
   f.separator = PROCESS_SEPARATOR;
   /* Read config into memory */
   proc_open(&f, PROCESS_COMMAND);

   /* Print header information */
   owi_header(PROCESS_HEADLINE);
   
   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print user list */
      process_list(&f);
   /* Show all process details */
   } else if (!strcmp(command, OWI_BUTTON_DETAIL)) {
      owi_detail_id(&f, variable_get("id"));
   }

   /* Free process list */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn process_list()
 * Show process
 */
void process_list(struct file_t *f) {
   /* Print outside table content */
   owi_outside_open(OWI_LIST);

   /* Display table header */
   owi_table_header(f);

   /* Reset line to zero */
   f->line_current = 1;

   /* Reset search */
   f->line_search = 0;

   /* Loop through all entries from file */
   while (file_data_get_next(f)) {
      /* Display */
      owi_data_list(f);
   }

   /* Print Submit button */
   owi_outside_close(OWI_BUTTON_NEW);
}
