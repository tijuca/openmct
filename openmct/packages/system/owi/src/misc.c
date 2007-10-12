/* -*- mode: C; c-file-style: "gnu" -*- */
/* misc.c  misc functions
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
#include <regex.h>

/* \fn hex2byte(string, pos)
 * \param[in] string hex string
 * \param[in] pos how many chars from beginning should be parsed
 * \return integer value from hex string
 */
int hex2byte(char *string, int pos) {
   /* variable for loop */
   int i;
   /* base will increase after each loop */
   int base = 0x1;
   /* return value */
   int value = 0;
   /* calculate digit from ascii character */
   int digit = 0;

   /* no pos specified? */
   if (!pos) {
      /* use complete string */
      pos = strlen(string);
   } else {
      /* decrement because in C index start at zero */
      pos--;
   }

   /* loop through characters from end to start */
   for (i = pos; i >= 0; i--) {
      /* '0' <= string[x] <= '9'? */
      if (string[i] >= 0x30 && string[i] <= 0x39) {
          digit = string[i] - 0x30;
      /* 'A' <= string[x] <= 'F'? */
      } else if (string[i] >= 0x41 && string[i] <= 0x46) {
         digit = string[i] - 0x37;
      /* 'a' <= string[x] <= 'f'? */
      } else if (string[i] >= 0x61 && string[i] <= 0x66) {
         digit = string[i] - 0x57;
      /* invalid character? */
      } else {
         digit = -1;
      } 
      /* valid character found? */
      if (digit != -1) {
         /* calculate new value */
         value += digit * base;
         /* change base */
         base *= 0x10;
      }
   }
   /* return result */
   return value;
}

int match(char *string, char *pattern) {
   int status;
   regex_t re;

   if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB)) {
      return 0;
   }
   status = regexec(&re, string, 0, NULL, 0);
   regfree(&re);
   if (status) {
      return 0; 
   }
   return 1;
}
