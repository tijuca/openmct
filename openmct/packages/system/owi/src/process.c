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
#include "includes/data.h"
#include "includes/language.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/process.h"

struct data_t process_data[] = {
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
     "CPU",
     "CPU usage",
     "cpu",
     "cpu",
     NULL,
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_LIST,
     "MEM",
     "Memory usage",
     "mem",
     "mem",
     NULL,
     NULL
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
     "RSS",
     "RSS",
     "RSS",
     "RSS",
     NULL,
     NULL
   },

   {
     DATA_TYPE_TEXT,
     0,
     "TTY",
     "TTY",
     "TTY",
     "TTY",
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
     0,
     "start",
     "Start",
     "Start",
     "start",
     NULL,
     NULL,
   },

   {
     DATA_TYPE_TEXT,
     0,
     "Time",
     "Time",
     "Time",
     "Time",
     NULL,
     NULL
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

/* \fn process_main(argc, argv)
 * Process information
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int process_main(int argc, char **argv) {
   /* File structure */
   struct file_t file;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = PROCESS_SEPARATOR;
   /* Read config into memory */
   proc_open(&file, PROCESS_COMMAND);

   /* Set owi properties for display */
   owi.headline = PROCESS_HEADLINE;
   owi.file = &file;
   owi.file_init = NULL;
   owi.data = process_data;
   owi.data_init = NULL;
   owi.button = NULL;
   owi.flags = OWI_FLAG_ACTION | OWI_FLAG_ROW | OWI_FLAG_HIDE_NEW;

   /* Start main */
   owi_main(&owi);

   /* Free data file */
   file_free(&file);

   /* Return success */
   return 0;
}
