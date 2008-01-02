/* -*- mode: C; c-file-style: "gnu" -*- */
/* array.c  misc functions
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
#include <regex.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include "includes/string.h"
#include "includes/array.h"

void array_key_add(struct array_t **array, struct string_t *key, struct string_t *value) {
   unsigned int index = 0;
   struct string_t *mykey = NULL;
 
   if (!*array) {
      *array = (struct array_t *)malloc(sizeof(struct array_t));
      (*array)->length = 0;
      (*array)->element = NULL;
   } else {
      index = (*array)->length;
   }

   /* No key specified? */
   if (!key) {
      /* Use index as key */
      mykey = string_format("%d", index);
      /* Store pointer for our function */
      key = mykey;
   }

   if (*array) {
      (*array)->element =
         (struct array_element_t **)realloc((*array)->element,
	                                    ((*array)->length + 1) * sizeof(struct array_element_t *));
      if ((*array)->element) {
         (*array)->element[index] =
	    (struct array_element_t*)malloc(sizeof(struct array_element_t));
         if ((*array)->element[index]) {
	    (*array)->element[index]->key = string_copy(key);
	    (*array)->element[index]->value = string_copy(value);
            (*array)->length++;
	 }
      }
   }

   if (mykey) {
      string_free(&mykey);
      mykey = NULL;
   }
}

void array_key_del(struct array_t **array, struct string_t *key) {
   unsigned int index = 0;

   /* Loop through all array elements */
   for (index = 0; index < (*array)->length; index++) {
      /* Index matches? */
      if (!string_compare(array_key(*array, index), key)) {
         /* Remove */
         array_index_del(array, index);
      }
   }
}

struct string_t *array_key_get_value(struct array_t *array, char *key) {
   unsigned int index = 0;
   struct string_t *value = NULL;

   /* Loop through all array elements */
   for (index = 0; index < array->length; index++) {
      /* Index matches? */
      if (!string_compare_value(array_key(array, index), key)) {
         /* Set return value */
         value = array_value(array, index);
      }
   }
   
   return value;
}

void array_index_del(struct array_t **array, unsigned int index) {
   unsigned int i = 0;

   if (index < (*array)->length) {
      /* Free element */
      string_free(&array_key(*array, index));
      string_free(&array_value(*array, index));
      /* Free element itself */
      free(array_element(*array, index));
      array_element(*array, index) = NULL;
      /* Move index after remove elements one step up */
      for (i = index; i + 1 < (*array)->length; i++) {
         array_element(*array, i) = array_element(*array, i + 1);
      }
      /* Decrease size */
      (*array)->length--;
      /* Reallocate memory */
      (*array)->element =
         (struct array_element_t **)realloc((*array)->element,
                                            ((*array)->length + 1) * sizeof(struct array_element_t *));
   }
}

void array_key_set(struct array_t *array, struct string_t *key, struct string_t *value) {
   /* Loop */
   unsigned int index = 0;

   /* Loop through all array elements */
   for (index = 0; index < array->length; index++) {
      /* Index matches? */
      if (!string_compare(array_key(array, index), key)) {
         /* Set value */
         array_value(array, index) = value;
      }
   }
}

void array_index_set(struct array_t *array, unsigned int index, struct string_t *value) {
   if (index < array->length) {
      array_value(array, index) = value;
   }
}

void array_free(struct array_t **array) {
   unsigned int index = 0;

   if (*array) {
      for (index = 0; index < (*array)->length; index++) {
         string_free(&array_key(*array, index));
         string_free(&array_value(*array, index));
	 free(array_element(*array, index));
      }
      (*array)->length = 0;
      (*array)->element = NULL;
      free(*array);
      *array = NULL;
   }
}

/* \fn string_split_value(array, string, separator)
 * Split a string into pieces separated with one or more characters
 * \param[in] string string that will be parsed
 * \param[in] separator separator that will be used for split
 * \return array of char pointers with string values
 */
struct array_t *string_split_value(
   struct string_t *value,
   char *separator) {

   /* index for string */
   int i;
   /* state for parser */
   enum STRING_SPLIT_STATE state = STRING_SPLIT_PRE;
   /* stop character */
   char stop_character = 0;
   /* start index for argument */
   int start_index = 0;
   /* stop index for argument */
   int stop_index = 0;
   /* first argument character position */
   int first_index = 0;
   /* String */
   struct string_t *string = NULL;
   /* Array */
   struct array_t *array = NULL;

   /* set index to zero */
   i = 0;
   /* while no end state was reached */
   while (state != STRING_SPLIT_END && state != STRING_SPLIT_INVALID && value != NULL) {
      /* get current state */
      switch(state) {
         /* pre parsing? */
         case STRING_SPLIT_PRE:
	      first_index = i;
              /* skip separator signs */
              while (i < string_length(value) &&
	             strchr(separator, string_char_at(value, i))) {
                 /* increase pointer */
                 i++;
              }
	      /* Two separator signs after each other */
	      if (first_index + 1 < i &&
	          string_char_at(value, first_index) == 
		  string_char_at(value, i - 1)) {
                 state = STRING_SPLIT_ADD;
		 start_index = i;
		 stop_index = i;
              /* end of string reached? */
              } else if (i == string_length(value)) {
                 /* set end state */
                 state = STRING_SPLIT_INVALID;
              } else {
                 /* next character "? */
                 if (string_char_at(value, i) == '"' ||
                     string_char_at(value, i) == '\'') {
                     /* stop character is 
                      * current character */
                    stop_character = string_char_at(value, i);
                    /* increase index */
                    i++;
                 } else {
                    /* clear stop character */
                    stop_character = 0;
                 }
                 /* set start index for argument */
                 start_index = i;
                 /* switch to next state */
                 state = STRING_SPLIT_DATA;
              }                                        
              break;

         /* data parser itself */                                        
         case STRING_SPLIT_DATA:
              /* while not string end reached and no stop
               * character found */
              while (i < string_length(value) &&
                     ((stop_character && stop_character != string_char_at(value, i)) ||
                     (!stop_character && !strchr(separator, string_char_at(value, i)))) ) {
                 /* parse through */
                 i++;
              }
              /* get data from start_index to stop_index */
              stop_index = i;
              /* stop character set? */                                        
              if (stop_character) {
                 /* skip next character */
                 i++;
              }
              /* switch to next state */
              state = STRING_SPLIT_ADD;
              break;
                        
         /* add argument to dynamic array now */                                
         case STRING_SPLIT_ADD:
	      /* Copy data */
	      string = string_extract(value,
                                      start_index,
                                      stop_index - start_index);
	      /* Add to array */
	      array_add(&array, string);
	      /* Reset to zero */
	      string = NULL;
              /* Set next state */
              state = STRING_SPLIT_PRE;
              break;

         /* invalid? */
         case STRING_SPLIT_INVALID:
              break;

         /* end? */
         case STRING_SPLIT_END:
              break;
      }                                
   }

   /* return array of elements */
   return array;
}

struct string_t *string_join_value(
   struct array_t *array,
   char *value) {

   /* Loop */
   unsigned int index;
   /* Result string */
   struct string_t *string = NULL;
   /* Tempoary string */
   struct string_t *old = NULL;

   /* Go through all array elements */
   for (index = 0; index < array_length(array); index++) {
      /* Append data */
      string = string_append(old, array_value(array, index));
      /* Free old string */
      string_free(&old);
      /* Set new old string for next append command */
      old = string;
      /* Do we need a separator now? */
      if (index + 1 < array_length(array)) {
         /* Append the separator */
         string = string_append_value(old, value);
         /* Free old string */
         string_free(&old);
         /* Set new old string for next append command */
	 old = string;
      }
   }

   return string;
}
