/* -*- mode: C; c-file-style: "gnu" -*- */
/* variable.c Variable handling functions
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "includes/variable.h"

/* dynamic array holds all variables */
struct variable_t **variable = NULL;

/* \fn variable_get(name)
 * Lookup variable in memory (return empty string if not set)
 * \param[in] name variable name
 * \return variable value
 */
char *variable_get(char *name) {
   /* variable for loop */
   int i;

   /* loop through array */
   for (i = 0; variable && variable[i] != NULL; i++) {
      /* variable entry set and variable name set? */
      if (variable[i] &&
          variable[i]->name &&
          !strcmp(variable[i]->name, name)) {
          return variable[i]->value;
      }
   }

   /* not found? */
   return "";
}

/* \fn variable_set(name, value, level)
 * Set variable in memory for a specified level
 * \param[in] name variable name
 * \param[in] value variable value
 */
void variable_set(char *name, char *value) {
   /* variable for loop */
   int i;
   /* variable found in array? */
   int found = 0;
   /* if index >= 0 this will be the index for the new element */
   int index_add = -1;
        
   /* already elements in variable array? */
   if (variable) {
      /* loop through array */
      for (i = 0; variable[i] != NULL; i++) {
      /* variable found? */
         if (variable[i] &&
             variable[i]->name &&
             !strcmp(variable[i]->name, name)) {
            /* free previous value */
            free(variable[i]->value);
            /* allocate memory for new value */
            variable[i]->value = (char*)malloc(strlen(value) + 1);
            /* set to zero */
            memset(variable[i]->value, 0, strlen(value) + 1);
            /* copy value */
            strcpy(variable[i]->value, value);
            /* set found to 1 */
            found = 1;
            /* break because already found */
            break;
         }
      }
      /* variable not found? add new? */
      if (!found) {
         index_add = i;
         /* reallocate memory for new element plus one for last element */
         variable = (struct variable_t **)realloc(variable,
                     sizeof(struct variable_t *) * (i + 2));
         /* set last element to zero */
         variable[i + 1] = NULL;
         variable[i] = NULL;
      }
   } else {
      variable = malloc(sizeof(struct variable *) * 2);
      /* add a new element */
      index_add = 0;
      /* set end pointer */
      variable[1] = NULL;
   }
   /* add a new element? */
   if (index_add != -1) {
      /* memory allocation ok? */
      if (variable) {
         variable[index_add] =
            (struct variable_t *)malloc(sizeof(struct variable_t));
         /* allocate memory for variable name */
         variable[index_add]->name = (char*)malloc(strlen(name) + 1);
         /* copy variable name */
         strcpy(variable[index_add]->name, name);
         /* allocate memory for value */
         variable[index_add]->value = (char*)malloc(strlen(value) + 1);
         /* copy value */
         strcpy(variable[index_add]->value, value);
      }
   }
}

/* \fn variable_error_set(variable, value)
 * Set error variable for another variable
 * \param[in] variable
 * \param[in] value
 */
void variable_error_set(char *variable, char *value) {
   /* Allocate memory for error variable name */
   char *error_variable = (char*)malloc(strlen(VARIABLE_ERROR) + strlen(variable) + 1);

   /* Enouqh memory? */
   if (error_variable) {
      /* Set error variable name */
      strcpy(error_variable, VARIABLE_ERROR);
      strcat(error_variable, variable);
      /* Assign value */
      variable_set(error_variable, value);
      free(error_variable);
   }
}

/* \fn variable_error_get(variable)
 * Get error variable for antoher variable
 * \param[in] variable
 * \return variable value
 */
char *variable_error_get(char *variable) {
   /* Allocate memory for error variable name */
   char *error_variable = (char*)malloc(strlen(VARIABLE_ERROR) + strlen(variable) + 1);
   /* Return value */
   char *ret = "";

   /* Enouqh memory? */
   if (error_variable) {
      /* Set error variable name */
      strcpy(error_variable, VARIABLE_ERROR);
      strcat(error_variable, variable);
      ret = variable_get(error_variable);
      free(error_variable);     
   }
  
   /* Return result */
   return ret;
}

/* \fn variable_dump()
 * Dump complete variable information from memory
 */
void variable_dump() {
   /* variable for loop */
   int i;

   printf("<code><pre>\n");
   /* loop through array */
   for (i = 0; variable && variable[i] != NULL; i++) {
      printf("[%03d] [%s] [%s]\n",
              i,
              variable[i]->name,
              variable[i]->value);
   }
   printf("</pre></code>\n");
}

/* \fn variable_free()
 * Free all variables
 */
void variable_free() {
   int i;
        
   /* already elements in variable array? */
   if (variable) {
      /* loop through array */
      for (i = 0; variable[i] != NULL; i++) {
         /* free variable name */
         free(variable[i]->name);
         /* set variable name to zero */
         variable[i]->name = NULL;
         /* free value */
         free(variable[i]->value);
         /* set value to zero */
         variable[i]->value = NULL;
         /* set level to zero */
         variable[i]->level = 0;
         /* free object itself */
         free(variable[i]);
         /* set object to zero */
         variable[i] = NULL;
      }
      /* free complete array */
      free(variable);
      /* set array to zero */
      variable = NULL;
   }
}

/* \fn variable_filter(variable, filter)
 * \param[in] variable Variable value
 * \param[in] filter Value filter
 */
char *variable_filter(char *variable, char *filter) {
   /* Index counter */
   int i;
   /* Variable set? */
   if (variable) {
      /* Filter value from variable */
      for (i = 0; i < strlen(variable); i++) {
          /* Unwanted character found? */
          if (strchr(filter, variable[i])) {
	     /* "Filter" it */
	     strcpy(variable + i, variable + i + 1);
	  }
      }
   }
   /* Return original pointer */
   return variable;
}

/* \fn variable_ltrm(variable, filter)
 * \param[in] variable Variable vale
 */
char *variable_ltrim(char *variable) {
   /* Variable set? */
   if (variable) {
      /* While leading spacing or tabs */
      while(*variable == ' ' || *variable == '\t') {
         /* Skip first character */
         strcpy(variable, variable + 1);
      }
   }
   /* Return original pointer */
   return variable;
}
