/* -*- mode: C; c-file-style: "gnu" -*- */
/* harddisk.c User management
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
 * MEHARDDISKHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
#include "includes/harddisk.h"

/* Define ini configuration tags */
struct file_data_t harddisk_data[] = {
   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "device",
     HARDDISK_NAME_DEVICE,
     NULL,
     NULL,
     "^[A-Za-z0-9/]{1,40}$",
     "OPTIONS_HDPARM_DEVICE",
     0,
     "/dev/discs/disc0/disc",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "dma",
     HARDDISK_NAME_DMA,
     NULL,
     NULL,
     "1|0",
     "OPTIONS_HDPARM_DMA",
     0,
     "1",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "interrupt_unmask",
     HARDDISK_NAME_INTERRUPT_UNMASK,
     NULL,
     NULL,
     "1|0",
     "OPTIONS_HDPARM_INTERRUPT_UNMASK",
     0,
     "1",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_SELECT,
     -1,
     "sector_count",
     HARDDISK_NAME_SECTOR_COUNT,
     NULL,
     NULL,
     "2|4|8|16",
     "OPTIONS_HDPARM_SECTOR_COUNT",
     0,
     "16",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_CHECKBOX,
     -1,
     "32bit",
     HARDDISK_NAME_32BIT,
     NULL,
     NULL,
     "3|0",
     "OPTIONS_HDPARM_32BIT",
     0,
     "3",
     FILE_DATA_FLAG_UPDATE
   },

   {
     FILE_DATA_TYPE_TEXT,
     -1,
     "spindown_timeout",
     HARDDISK_NAME_SPINDOWN_TIMEOUT,
     NULL,
     NULL,
     "^[0-9]{1,3}$",
     "OPTIONS_HDPARM_SPINDOWN_TIMEOUT",
     0,
     "60",
     FILE_DATA_FLAG_UPDATE
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

/* \fn harddisk_main(argc, argv)
 * Show all harddisks from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int harddisk_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   /* File structure */
   struct file_t f;

   /* Set correct type */
   f.type = FILE_TYPE_SECTION;
   /* Set config settings */
   f.fd = harddisk_data;
   /* Set separator */
   f.separator = HARDDISK_SEPARATOR;
   /* Read config into memory */
   file_open(&f, HARDDISK_FILE);
 
   /* Print header information */
   owi_header(HARDDISK_HEADLINE);

   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print harddisk list */
      owi_list(&f, NULL);
   } else if (!strcmp(command, OWI_BUTTON_UPDATE)) {
      /* Update configuration failed */
      owi_update(&f, HARDDISK_FILE_UPDATE, HARDDISK_FILE_ERROR);
      /* Display harddisk details again */
      owi_list(&f, NULL);
   } 
   /* Free file */
   file_free(&f);

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}
