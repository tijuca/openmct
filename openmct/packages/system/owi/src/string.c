/* -*- mode: C; c-file-style: "gnu" -*- */
/* string.c  string functions
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
#include <regex.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include "includes/string.h"

/* \fn string_format(format, ...)
 * \param[in] format format string for new string
 * \param[in] ... arugments for format string
 * \return new string object with content
 */
struct string_t *string_format(
   char *format,
   ...) {

   /* Temporary string */
   char *temp = NULL;
   /* Variable argument handling */
   va_list va;
   /* Correct size can't be detected */
   unsigned int correctsize = 0;
   /* Start length */
   unsigned int length = 1024;
   /* Destination */
   struct string_t *destination = NULL;

   /* Get the real length for evaluated format string */
   do {
      /* Allocate memory for temporary string */
      temp = (char*)realloc(temp, length);
      /* Enough memory? */
      if (temp) {
         /* Get variable number of arguments after format */
         va_start(va, format);
	 /* Write data to temp */
         vsnprintf(temp, length, format, va);
	 /* Just for correct argument handling */
         va_end(va);
         
	 /* string length smaller the requested size? */
         if (strlen(temp) < length) {
	    /* So we have string length */
            correctsize = 1;
	    /* Set correct string length in now */
	    if (length > strlen(temp)) {
               length = strlen(temp);
	    }
         } else {
	    /* Allocate another 1024 to check if this is enough space */
            length += 1024;
         }
      /* Not enough memory? */
      } else {
         /* TODO: cleanup code */
         /* Kick off ;) */
         exit(-1);
      }
   /* Loop until we've got the correct string size */
   } while (!correctsize);

   /* Allocate object for new string */
   destination = string_copy_value(temp);

   /* Free temporary format string */
   free(temp);

   /* Return new string object */
   return destination;
}

/* \fn string_move(old, destination_offset, source_offset, length, value)
 * \param[in] old old string object with content (if required)
 * \param[in] destination_offset index from target
 * \param[in] source_offset index from source
 * \param[in] value string value that will be written
 * \return new string object with content
 */
struct string_t *string_move(
   struct string_t *old, 
   unsigned int destination_offset,
   unsigned int source_offset,
   unsigned int length,
   char *value) {

   struct string_t *destination = NULL;

   /* Allocate memory for string object */
   destination = (struct string_t*)malloc(sizeof(struct string_t));
   if (!destination) {
      /* TODO: cleanup code */
      exit(-1);
   }
   /* Set new string length */
   destination->length = length;
   /* old string set? */
   if (old) {
      /* Increase length */
      destination->length += old->length;
   }

   /* Allocate memory for string value */
   destination->value = (char*)malloc(destination->length + 1);
   /* Enough memory? */
   if (destination->value) {
      if (old) {
         /* Copy old string */
         strncpy(destination->value,
	         string_value(old),
		 string_length(old) > destination_offset ?
		 destination_offset : string_length(old));
      }
      /* Source string set? */
      if (value) {
         /* Copy string */
         strncpy(destination->value + destination_offset,
                 value + source_offset,
	         length);
         /* Terminate string */
         destination->value[destination->length] = 0;
      } else {
         /* No source string definied so wie reset all bytes to zero */
	 memset(destination->value, 0, length);
      }
      /* Do we have to add a too? */
      if (old && string_length(old) > destination_offset) {
         strncpy(destination->value + destination_offset + length,
	         string_value(old) + destination_offset,
		 string_length(old) - destination_offset);
      }
   } else {
      exit(-1);
   }
   
   /* Return new string */
   return destination;
}

/* \fn string_free(string)
 * Free memory that is allocated by this object
 * \param[in|out] string string object that will be free'd
 */
void string_free(struct string_t **string) {
   /* String? */
   if (*string) {
      /* Clear memory and set write zero bytes */
      memset((*string)->value, 0, (*string)->length);
      /* Free content */
      free((*string)->value);
      /* and sets to zero */
      (*string)->value = NULL;
      /* Reset length to zero */
      (*string)->length = 0;
      /* Set pointer to zero */
      *string = NULL;
   }
}

/* \fn string_decode(string)
 * Decode string object from http request (with %XX encoding)
 * \param[in] string string object that will be decoded
 * \return new string object
 */
struct string_t *string_decode(struct string_t *string) {
   /* Index for loop */
   unsigned int index;
   /* Hexadecimal string */
   struct string_t *hex = NULL;
   /* Character */
   unsigned int character;
   /* Result string */
   struct string_t *result = string_copy(string), *pre = NULL, *post = NULL;

   /* Loop through string */
   for (index = 0; index < string_length(result); index++) {
      /* Percent sign found and two characters are next to the percent sign? */
      if (string_char_at(result, index) == '%' &&
          index + 2 < string_length(result)) {
         /* Convert next two bytes to an character */
         hex = string_extract(result, index + 1, 2);
	 /* Convert to character */
	 sscanf(string_value(hex), "%4x", &character);
	 /* Free hex string */
	 string_free(&hex);
	 /* Set character now */
	 hex = string_format("%c", character);
         /* Extract all until % sign */
	 pre = string_extract(result, 0, index);
	 /* Extract all from % until string end */
	 post = string_extract(result,
	                       index + 3,
			       string_length(result) - index - 3);
	 /* Free result string */
	 string_free(&result);
	 /* Append data */
	 result = string_append(pre, hex);
	 /* Free hex string */
	 string_free(&hex);
	 /* Free pre string */
	 string_free(&pre);
	 /* Use pointer from result as current pre (includes hex digit) */
	 pre = result;
	 result = string_append(pre, post);
	 /* Free post string */
	 string_free(&post);
	 /* Free pre string */
	 string_free(&pre);
      /* Replace + with a space */
      } else if (string_char_at(result, index) == '+') {
         string_char_at(result, index) = ' ';
      }
   }

   /* Return result */
   return result;
}

/* \fn string_match_value(string, pattern)
 * \param[in] string string object that will be checked against pattern
 * \param[in] pattern regular expression
 * \return 1 if regular expression matched or 0 if not
 */
unsigned int string_match_value(struct string_t *string, char *pattern) {
   /* Status for regular expression */
   int status;
   /* Regular expression */
   regex_t re;

   /* Compile regular expression */
   if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB)) {
      return 0;
   }
 
   /* Verify regular expression */
   status = regexec(&re, string_value(string), 0, NULL, 0);

   /* Free structure for regular expression */
   regfree(&re);

   if (status) {
      return 0; 
   }

   return 1;
}
