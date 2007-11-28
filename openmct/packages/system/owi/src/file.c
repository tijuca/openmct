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
#include "includes/file.h"
#include "includes/argument.h"
#include "includes/variable.h"
#include "includes/misc.h"
#include "includes/language.h"

/* \fn file_open(f, filename)
 * \param[in] f file structure for current file
 * \param[in] filename text file that will be read
 */
void file_open(struct file_t *f, char *filename) {
   /* File handle */
   FILE *fp = NULL;

   /* Set initial line counter to zero */
   f->line_count = 0;
   /* Set line to zero */
   f->line = NULL;

   /* Allocate memory for filename */
   f->name = (char*)malloc(strlen(filename) + strlen(FILE_BASE) + 1);

   if (f) {
      /* Malloc ok? */
      if (f->name) {
         /* Write real filename into variable */
         sprintf(f->name, "%s%s", FILE_BASE, filename);
         /* Try to open file */
         fp = fopen(f->name, "r");
         /* Successful? */
         if (fp) {
            /* Read complete file into structure */
            file_read(f, fp);
            /* Close file now */
            fclose(fp);
         }
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

   /* Loop through file */
   while (fgets(line, sizeof(line), fp)) {
      /* Strip \n from line */
      line[strlen(line) - 1] = 0;
      /* Reallocate array */
      f->line = (struct file_line_t**)
                realloc(f->line,
                       sizeof(struct file_line_t *) * (f->line_count + 1));
      /* Enough memory? */
      if (f->line) {
         /* Allocate memory for line */
         f->line[f->line_count] = (struct file_line_t *)
	                          malloc(sizeof(struct file_line_t));
         /* Enough memory? */
         if (f->line[f->line_count]) {
            /* Copy line to data */
            f->line[f->line_count]->data = strdup(line);
	    /* Parse data */
	    f->line[f->line_count]->current =
	        argument_parse(f->line[f->line_count]->data,
		               f->separator);
            /* Not changed per default */
	    f->line[f->line_count]->changed = 0;
            /* Increase line counter */
            f->line_count++;
         }
      }
   }
}

/* \fn file_free(f, fp)
 * Free file content for current file structure
 * \param[in] f file structure for current file
 */
void file_free(struct file_t *f) {
   /* Loop */
   int i = 0;

   /* Data lines available? */
   if (f->line) {
      /* Loop through all lines */
      for (i = 0; i < f->line_count; i++) {
         /* Free raw line */
         free(f->line[i]->data);
	 /* Free current values */
	 argument_free(f->line[i]->current);
	 /* and set to zero */
         f->line[i] = NULL;
      }
      /* Free array pointer for lines */
      free(f->line);
      /* Set array pointer to zero */
      f->line = NULL;
   }

   /* Free space for filename */
   free(f->name);
   /* Set to zero */
   f->name = NULL;
}

/* \fn file_save(f)
 * Save lines to this file
 * \param[in] filename data will be written to this file
 */
void file_save(struct file_t *f) {
   /* File handle */
   FILE *fp = NULL;
   /* Loop */
   int i = 0;
   /* Changed lines */
   int change = 0;

   /* Look for file changes */
   for  (i = 0; i < f->line_count; i++) {
      if (f->line[i]->changed) {
         change++;
      }
   }

   /* Changed lines found? */
   if (change) {
      /* Update raw lines for writing */
      file_data_update(f);

      /* Try to open file for writing */
      fp = fopen(f->name, "w");
      /* Successful? */
      if (fp) {
         /* Loop through all lines */
         for (i = 0; i < f->line_count; i++) {
            fprintf(fp, "%s\n", f->line[i]->data);
         }
         /* Close file */
         fclose(fp);
      }
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
   char line[FILE_MAXLINE];
   /* Index counter */
   int i;

   /* Format is not null? */
   if (format != NULL) {
      /* Get variables from stack */
      va_start(va, format);
      /* And write them to line */
      vsnprintf(line, FILE_MAXLINE, format, va);
      /* Clean up for argument handling */
      va_end(va);
   }
 
   /* Set line? */
   if (mode == FILE_LINE_SET) {
      /* Free old data */
      free(f->line[f->line_current]->data);
      /* Allocate data for new line and copy it */
      f->line[f->line_current]->data = strdup(line);
   /* Del line? */        
   } else if (mode == FILE_LINE_DEL) {
      /* Free current line */
      free(f->line[f->line_current]->data);
      /* Free current line */
      argument_free(f->line[f->line_current]->current);
      /* Free space for array index */
      free(f->line[f->line_current]);
      /* Copy every element after del index to its previous position */
      for (i = f->line_current; i < f->line_count - 1; i++) {
         /* Just set pointer */              
         f->line[i] = f->line[i + 1];
      }
      /* Decrease line counter */
      f->line_count--;
      /* Reallocate new index array */
      f->line = (struct file_line_t **)realloc(f->line,
                                       sizeof(struct file_line_t *) *
                                       (f->line_count + 1));
   /* Add line? */         
   } else if (mode == FILE_LINE_ADD) {
      /* Increase line counter */
      f->line_count++;
      /* Reallocate new index array */
      f->line = (struct file_line_t **)realloc(f->line,
                                       sizeof(struct file_line_t *) *
                                       (f->line_count + 1));
      /* Move pointers */
      for (i = f->line_count - 1; i > f->line_current; i--) {
         f->line[i] = f->line[i - 1];
      }

      /* Allocate space for new index */
      f->line[f->line_current] = (struct file_line_t *)
                                 malloc(sizeof(struct file_line_t));
      /* Copy data */
      f->line[f->line_current]->data = strdup(line);
      /* Parse data */
      f->line[f->line_current]->current = argument_parse(line, f->separator);
      /* Not changed per default */
      f->line[f->line_count]->changed = 0;
   } 
}

/* \fn file_data_update(f)
 * Convert from current data storage to raw line
 */
void file_data_update(struct file_t *f) {
   /* Index */
   int i, j;
   /* Line length */
   int line_length = 0;

   /* Loop through whole file */
   for (j = 0; j < f->line_count; j++) {
      /* No change in this line? */
      if (!f->line[j]->changed)
         continue;

      /* Free current raw line */
      free(f->line[j]->data);
      /* Set to zero */
      f->line[j]->data = NULL;

      /* Loop through all settings */
      for (i = 0; f->line[j]->current[i] != NULL; i++) {
         /* Increase line lenght */
         line_length += strlen(f->line[j]->current[i]);

         /* Need separator? */
         if (f->line[j]->current[i + 1] != NULL) {
            line_length += strlen(f->separator);
         }

         /* Allocate space for line */
         f->line[j]->data = realloc(f->line[j]->data,
                                    line_length + 1);
         /* Enough space? */
         if (f->line[j]->data) {
            /* First realloc? */
            if (!i) {
               /* Clear memory first */
               memset(f->line[j]->data, 0, line_length + 1);
            }
            strcat(f->line[j]->data, f->line[j]->current[i]);
            /* Need separator? */
            if (f->line[j]->current[i + 1] != NULL) {
               strcat(f->line[j]->data, f->separator);
            }
         }
      }
   }
}

/* \fn file_value_set(f, name_index, name, value_index, value)
 * \param[in] f file structure for current file
 * \param[in] name_index current index for name lookup
 * \param[in] name variable name
 * \param[in] value_index current index for value
 * \param[in] value variable value
 */
void file_value_set(struct file_t *f, int name_index, char *name, int value_index, char *value, int changed) {
   /* Loop */
   int i;

   /* Only update if both values are valid */
   if (name && value) {
      /* Loop through all lines in file */
      for (i = 0; i < f->line_count; i++) {
         /* Pattern found? */
         if (!strcasecmp(f->line[i]->current[name_index], name)) {
            /* Value already set and not the same? */
            if (f->line[i]->current[value_index] != NULL) {
               /* Only free old value */
               free(f->line[i]->current[value_index]);
               /* And copy new value */
               f->line[i]->current[value_index] = strdup(value);
	       /* Set change modus */
	       f->line[i]->changed = changed;
	    }
	 }
      }
   }
}

/* \fn file_value_get(f, name_index, name, value_index, value)
 * \param[in] f file structure for current file
 * \param[in] name_index current index for name lookup
 * \param[in] name variable name
 * \param[in] value_index current index for value
 */
char *file_value_get(struct file_t *f, int name_index, char *name, int value_index) {
   /* Loop */
   int i;
   /* Current value */
   char *current = NULL;

   /* Loop through all lines in file */
   for (i = 0; i < f->line_count; i++) {
      /* Pattern found? */
      if (f->line[i]->current[name_index] &&
          !strcasecmp(f->line[i]->current[name_index], name) &&
          f->line[i]->current[value_index] != NULL) {
         current =  f->line[i]->current[value_index];
      }
   }

   /* Return current value */
   return current;
}

/* \fn proc_open(f, command)
 * \param[in] f file structure for current file
 * \param[in] command will be executed
 */
void proc_open(struct file_t *f, char *command) {
   /* File handle */
   FILE *fp = NULL;

   /* Set initial line counter to zero */
   f->line_count = 0;
   /* Set line to zero */
   f->line = NULL;

   /* Set name to null */
   f->name = NULL;

   if (f) {
      /* Execute command */
      fp = popen(command, "r");
      /* Successful? */
      if (fp) {
         /* Read complete file into structure */
         file_read(f, fp);
         /* Close file now */
         pclose(fp);
      }
   }
}
