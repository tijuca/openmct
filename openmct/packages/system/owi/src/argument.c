/* -*- mode: C; c-file-style: "gnu" -*- */
/* argument.c Argument / tag command parser
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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/argument.h"

/* \fn argument_parse_seperator(value, seperator)
 * Split a string into pieces seperated with one or more characters
 * \param[in] value string that will be parsed
 * \param[in] seperator seperator that will be used for split
 * \return array of char pointers with string values
 */
char **argument_parse(char *value, char *seperator) {
   /* index for string */
   int i;
   /* state for parser */
   enum ARGUMENT_STATE state = ARGUMENT_PRE;
   /* stop character */
   char stop_character = 0;
   /* start index for argument */
   int start_index = 0;
   /* stop index for argument */
   int stop_index = 0;
   /* Argument counter */
   int argument_count = 0;
   /* argument data */
   char **argument = NULL;

   /* set index to zero */
   i = 0;
   /* while no end state was reached */
   while (state != ARGUMENT_END && state != ARGUMENT_INVALID && value != NULL) {
      /* get current state */
      switch(state) {
         /* pre parsing? */
         case ARGUMENT_PRE:
              /* skip seperator signs */
              while (i < strlen(value) && strchr(seperator, value[i])) {
                 /* increase pointer */
                 i++;
              }
              /* end of string reached? */
              if (i == strlen(value)) {
                 /* set end state */
                 state = ARGUMENT_INVALID;
              } else {
                 /* next character "? */
                 if (value[i] == '"' ||
                     value[i] == '\'') {
                     /* stop character is 
                      * current character */
                    stop_character = value[i];
                    /* increase index */
                    i++;
                 } else {
                    /* clear stop character */
                    stop_character = 0;
                 }
                 /* set start index for argument */
                 start_index = i;
                 /* switch to next state */
                 state = ARGUMENT_DATA;
              }                                        
              break;

         /* data parser itself */                                        
         case ARGUMENT_DATA:
              /* while not string end reached and no stop
               * character found */
              while (i < strlen(value) &&
                     ((stop_character && stop_character != value[i]) ||
                     (!stop_character && !strchr(seperator, value[i]))) ) {
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
              state = ARGUMENT_ADD;
              break;
                        
         /* add argument to dynamic array now */                                
         case ARGUMENT_ADD:                        
              /* increase argument counter */
              argument_count++;
              /* allocate space for argument data */
              argument = (char**)realloc(argument, sizeof(char *) *
                                         argument_count);
              /* allocate ok? */
              if (argument) {
                 /* allocate memory */
                 argument[argument_count - 1] = (char*)malloc(stop_index -
                                                start_index + 1);
                 /* malloc ok? */
                 if (argument[argument_count - 1]) {
                    /* clear buffer */
                    memset(argument[argument_count - 1], 0,
                           stop_index - start_index + 1);
                    /* copy value now */
                    strncpy(argument[argument_count - 1],
                            value + start_index,
                            stop_index - start_index);
                 } else {
                    /* memory error */
                    state = ARGUMENT_MEMORY;
                 }
                 /* set next state */
                 state = ARGUMENT_PRE;
              } else {
                 /* memory error */
                 state = ARGUMENT_MEMORY;
              }
              break;

         /* memory error */
         case ARGUMENT_MEMORY:
              /* just go to end state */
              state = ARGUMENT_INVALID;
              break;

         /* invalid? */
         case ARGUMENT_INVALID:
              break;

         /* end? */
         case ARGUMENT_END:
              break;
      }                                
   }
   /* increase couter for last NULL element */
   argument_count++;
   /* allocate space for last NULL entry */
   argument = (char**)realloc(argument, sizeof(char *) * argument_count);
   /* set last entry to NULL */
   argument[argument_count - 1] = NULL;
   /* return array of elements */
   return argument;
}

/* \fn argument_free(argument)
 * Free dynamic array of arguments
 * \param[in] argument dynamic array of char pointers
 */
void argument_free(char **argument) {
   /* Variable for loop */
   int i;

   /* Loop through all arguments */
   for (i = 0; argument[i] != NULL; i++) {
      /* Free element */
      free(argument[i]);
      /* Set to NULL */
      argument[i] = NULL;
   }
   /* free dynamic array */
   free(argument);
   /* Set pointer to NULL */
   *argument = NULL;
}

/* \fn argument_get(argument, index, seperator)
 * Get one string containng all arguments separated with seperator
 * \param[in] argument argument data
 * \param[in] index start at this index
 * \param[in] seperator use this string as seperator 
 * \return pointer to string
 */
char *argument_get(char **argument, int index, char *seperator) {
   /* Option data (from 1st argument until n-th argument) */
   char *string = NULL;
   /* Define whole string len for all options */
   int string_length = 0;
   /* Index counter */
   int i = 0;

   /* Loop through all argument values */
   for (i = index; argument[i] != NULL; i++) {
      /* Calculate whole length */
      string_length += strlen(argument[i]) + strlen(seperator);
   }

   /* Allocate memory for options */
   string = (char*)malloc(string_length + 1);
   /* Everything ok? */
   if (string) {
      /* Clear buffer */
      memset(string, string_length + 1, 0);
      /* Loop through all argument values */
      for (i = index; argument[i] != NULL; i++) {
         /* Append data to string */
         strcat(string, argument[i]);
         /* Append seperator to string */
         strcat(string, seperator);
      }
   } 
   
   /* Return result */
   return string;
}

/* \fn argument_get_part(argument, index)
 * \param[in] argument Argument data
 * \param[in] index get this index
 */
char *argument_get_part(char **argument, int index) {
   /* Index counter */
   int i;

   for (i = 0; argument[i] != NULL; i++) {
      if (index == i) {
         break;
      }
   }

   return argument[i] ? argument[i] : "";
}
