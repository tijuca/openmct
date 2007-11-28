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
#include "includes/data.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/harddisk.h"

/* Define ini configuration tags */
struct data_t harddisk_data[] = {
   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_DEVICE,
     NULL,
     "device",
     "OPTIONS_HDPARM_DEVICE",
     "^[A-Za-z0-9/]{1,40}$",
     "/dev/discs/disc0/disc"
   },

   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_DMA,
     NULL,
     "dma",
     "OPTIONS_HDPARM_DMA",
     "1|0",
     "1"
   },

   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_INTERRUPT_UNMASK,
     NULL,
     "interrupt_unmask",
     "OPTIONS_HDPARM_INTERRUPT_UNMASK",
     "1|0",
     "1"
   },

   {
     DATA_TYPE_SELECT,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_SECTOR_COUNT,
     NULL,
     "sector_count",
     "OPTIONS_HDPARM_SECTOR_COUNT",
     "2|4|8|16",
     "16"
   },

   {
     DATA_TYPE_CHECKBOX,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_32BIT,
     NULL,
     "32bit",
     "OPTIONS_HDPARM_32BIT",
     "3|0",
     "3"
   },

   {
     DATA_TYPE_TEXT,
     DATA_FLAG_UPDATE,
     HARDDISK_NAME_SPINDOWN_TIMEOUT,
     NULL,
     "spindown_timeout",
     "OPTIONS_HDPARM_SPINDOWN_TIMEOUT",
     "^[0-9]{1,3}$",
     "60"
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

/* \fn harddisk_main(argc, argv)
 * Show all harddisks from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int harddisk_main(int argc, char **argv) {
   /* File structure */
   struct file_t file;
   /* owi structure */
   struct owi_t owi;

   /* Set separator */
   file.separator = HARDDISK_SEPARATOR;
   /* Read config into memory */
   file_open(&file, HARDDISK_FILE);

   /* Set owi properties for display */
   owi.headline = HARDDISK_HEADLINE;
   owi.file = &file;
   owi.file_init = NULL;
   owi.data = harddisk_data;
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
