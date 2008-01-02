/* -*- mode: C; c-file-style: "gnu" -*- */
/* shell.c User management
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
#include <crypt.h>
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/shell.h"

/* \fn shell_main(argc, argv)
 * Show all shells from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int shell_main(struct owi_t *owi) {
   owi->file->separator = string_copy_value("	");
   /* Read config into memory */
   owi->file->name = owi_get("execute");
   /* Set owi properties for display */
   owi->headline = string_copy_value(SHELL_HEADLINE);
   owi->flags = OWI_FLAG_CONFIG;
   /* Start main */
   owi_main(owi);
   /* Return success */
   return 0;
}
