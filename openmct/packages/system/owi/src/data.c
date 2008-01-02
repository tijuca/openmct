/* -*- mode: C; c-file-style: "gnu" -*- */
/* data.c  data functions
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
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/data.h"

/* \fn data_valid(data, value)
 * Valid entry for variable?
 */
int data_valid(struct data_t *data, struct string_t *value) {
   /* Valid values for checkbox */
   struct array_t *valid_values = NULL;
   /* Valid data */
   struct string_t *valid = string_copy_value(data->valid);
   /* Return value */
   int retval = 0;

   if (data->type == DATA_TYPE_CHECKBOX) {
      /* Valid values */
      valid_values = string_split_value(valid, "|");
      /* Valid? */
      if (!string_compare(array_value(valid_values, 0), valid) |
          !string_compare(array_value(valid_values, 1), valid)) {
         retval = 1;
      }
      /* Free values */
      array_free(&valid_values);
   } else {
      if (data->flags & DATA_FLAG_DONT_FILL &&
          (!value || !string_compare_value(value, ""))) {
         retval = 1;
      } else if ( (data->valid && value && string_match_value(value, data->valid)) ||
                  !data->valid) {
         retval = 1;
      }
   }

   /* Return retval value */
   return retval;
}
