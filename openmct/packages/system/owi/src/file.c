/* -*- mode: C; c-file-style: "gnu" -*- */
/* file.c General file handling
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
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <crypt.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "includes/language.h"
#include "includes/string.h"
#include "includes/array.h"
#include "includes/file.h"

/* \fn file_open(f, filename)
 * \param[in] f file structure for current file
 * \param[in] filename text file that will be read
 */
void file_open(struct file_t *f) {
   /* File handle */
   FILE *fp = NULL;
   /* Real filename */
   struct string_t *real_filename = NULL;

   if (f) {
      /* Set line array to null */
      f->line = NULL;
      /* Allocate memory for filename */
      real_filename = string_format("%s%s", FILE_BASE, string_value(f->name));
      /* Free current filename */
      string_free(&(f->name));
      /* Set new filename */
      f->name = real_filename;
      /* Try to open file */
      fp = fopen(string_value(f->name), "r");
      /* Successful? */
      if (fp) {
         /* Read complete file into structure */
         file_read(f, fp);
         /* Close file now */
         fclose(fp);
      }
   }
}

/* \fn file_read(f, fp)
 * \param[in] f file structure for current file
 * \param[in] fp file pointer
 */
void file_read(struct file_t *f, FILE *fp) {
   /* Each line from file */
   char line[FILE_MAXLINE];
   /* String */
   struct string_t *string = NULL;

   /* Loop through file */
   while (fgets(line, sizeof(line), fp)) {
      /* Strip \n from line */
      line[strlen(line) - 1] = 0;
      /* Allocate memory for line */
      string = string_copy_value(line);
      /* Add to array */
      array_add(&(f->line), string);
      /* Reset to zero so next string_set will allocate a new object 
       * don't use string_free here because only a reference will be added 
       * to the array */
      string = NULL;
   }
}

/* \fn file_free(f, fp)
 * Free file content for current file structure
 * \param[in] f file structure for current file
 */
void file_free(struct file_t *f) {
   /* Free space for filename */
   string_free(&(f->name));
   /* Free separator */
   string_free(&(f->separator));
   /* Free space for array */
   array_free(&(f->line));
   /* Free space for current parsed line in memory */
   array_free(&(f->current));
}

/* \fn file_save(f)
 * Save lines to this file
 * \param[in] filename data will be written to this file
 */
void file_save(struct file_t *f) {
   /* File handle */
   FILE *fp = NULL;
   /* Loop */
   unsigned int i = 0;

   /* Try to open file for writing */
   fp = fopen(string_value(f->name), "w");
   /* Successful? */
   if (fp) {
      /* Loop through all lines */
      for (i = 0; i < array_length(f->line); i++) {
         fprintf(fp, "%s\n", file_line(f, i) ?
	                     string_value(file_line(f, i)) : "");
      }
      /* Close file */
      fclose(fp);
   }
}

/* \fn file_action
 * Add/Set/Del a line from line array
 * \param[in] f file structure
 * \param[in] mode Delete, Add or Set line
 * \param[in] format format string for line
 * \param[in] ... Arguments on stack for *printf
 */
void file_action(struct file_t *f, int mode, char *format, ...) {
   /* Variable argument handling */
   va_list va;
   /* Buffer for this line */
   char line[FILE_MAXLINE] = "";
   /* String buffer */
   struct string_t *string = NULL;

   /* Format is not null? */
   if (format != NULL) {
      /* Get variables from stack */
      va_start(va, format);
      /* And write them to line */
      vsnprintf(line, FILE_MAXLINE, format, va);
      /* Clean up for argument handling */
     va_end(va);
   }
   string = string_copy_value(line);
 
   /* Set line? */
   if (mode == FILE_LINE_SET) {
      /* Set new element */
      array_index_set(f->line, f->line_current, string);
   /* Del line? */        
   } else if (mode == FILE_LINE_DEL) {
      array_index_del(&(f->line), f->line_current);
   /* Add line? */         
   } else if (mode == FILE_LINE_ADD) {
      array_add(&(f->line), string);
      string = NULL;
   } 
}

/* \fn file_value_set(f, name_index, name, value_index, value)
 * \param[in] f file structure for current file
 * \param[in] name_index current index for name lookup
 * \param[in] name variable name
 * \param[in] value_index current index for value
 * \param[in] value variable value
 */
void file_value_set(
   struct file_t *f,
   unsigned int name_index,
   struct string_t *name,
   unsigned int value_index,
   struct string_t *value) {

   /* Loop */
   int i;
   /* Current values */
   struct array_t *current = NULL;
   /* New string */
   struct string_t *string = NULL;

   /* Only update if both values are valid */
   if (name && value) {
      /* Loop through all lines in file */
      for (i = 0; i < array_length(f->line); i++) {
         struct string_t *line = file_line(f, i);
         /* Parse elements */
         current = string_split(line, f->separator);

         /* Copy string from array to target "string" */
         if (current &&
	     name_index < array_length(current) &&
	     value_index < array_length(current) &&
	     !string_compare(array_value(current, name_index), name)) {
            /* Free current line */
            string_free(&array_value(current, value_index));
	    /* Set new value in current array - we dont want a reference
	     * so we copy the whole string in memory */
	    array_index_set(current, value_index, string_copy(value));
	    /* Set new value in line array */
	    array_index_set(f->line, i, string_join(current, f->separator));
	    string = NULL;
         }

         /* Free string array */
         array_free(&current);
      }
   }
}

/* \fn file_value_get(f, name_index, name, value_index, value)
 * \param[in] f file structure for current file
 * \param[in] name_index current index for name lookup
 * \param[in] name variable name
 * \param[in] value_index current index for value
 * \return current value or null if not found
 */
struct string_t *file_value_get(
   struct file_t *f,
   unsigned int name_index,
   struct string_t *name,
   unsigned int value_index) {

   /* Loop */
   unsigned int i = 0;
   /* Current values */
   struct array_t *current = NULL;
   /* String for return */
   struct string_t *string = NULL;
   /* Current line */
   struct string_t *line = NULL;

   /* Only update if both values are valid */
   if (name) {
      /* Loop through all lines */
      for (file_iterator_start(f, &i) ;
           i < file_length(f);
           file_iterator_next(f, &i)) {
         /* Set current line */
         line = file_line(f, i);
         /* Parse elements */
         current = string_split(line, f->separator);
         /* Copy string from array to target "string" */
         if (current &&
	     name_index < array_length(current) && 
	     value_index < array_length(current) &&
	     !string_compare(array_value(current, name_index), name)) {
            string = string_copy(array_value(current, value_index));
         } 
         /* Free string array */
         array_free(&current);
      }
   }

   return string;
}

/* \fn file_value_del(f, name_index, name)
 * \param[in] f file structure for current file
 * \param[in] name_index current index for name lookup
 * \param[in] name variable name
 */
void file_value_del(
   struct file_t *f,
   int name_index,
   struct string_t *name) {
}

/* \fn file_next(f, index)
 * Get index for next line (here you can skip special lines)
 * \param[in] f file structure for current file
 * \param[in] index current index
 */
void file_iterator(struct file_t *f, unsigned int mode, unsigned int *index) {
   /* Store current line in this variable */
   struct string_t *line;

   /* During first line we are not allowed to increase */
   if (mode == FILE_ITERATOR_NEXT) {
      (*index)++;
   }
   
   /* Filter flags */
   if (f->flags & FILE_FLAG_SKIP_COMMENT) {
      /* While lines are available */
      while ( *index + 1 < file_length(f)) {
         /* Get next line */
         line = file_line(f, *index);
	 /* No comment line? */
         if (string_char_at(line, 0) != '#') {
	    /* Quit loop */
            break;
         }
	 /* Get next line */
         (*index)++;
      }
   }
}
