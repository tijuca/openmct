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
   /* Loop */
   int i = 0;

   /* Allocate memory for filename */
   f->name = (char*)malloc(strlen(filename) + strlen(FILE_BASE) + 1);

   /* Loop through all config settings */
   for (i = 0; f->fd[i].html != NULL; i++) {
      f->fd[i].current = NULL;
   }

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

   /* Set initial line counter to zero */
   f->line_count = 0;
   /* Set line to zero */
   f->line = NULL;
   /* Set current line to zero */
   f->line_current = 0;
   /* Set search line to zero */
   f->line_search = 0;

   /* Loop through file */
   while (fgets(line, sizeof(line), fp)) {
      /* Strip \n from line */
      line[strlen(line) - 1] = 0;
      /* Reallocate array */
      f->line = (char**)realloc(f->line,
                                sizeof(char *) * (f->line_count + 1));
      /* Enough memory? */
      if (f->line) {
         /* Allocate memory for line */
         f->line[f->line_count] = (char*)malloc(strlen(line) + 1);
         /* Enough memory? */
         if (f->line[f->line_count]) {
            /* Copy line to data */
            strcpy(f->line[f->line_count], line);
            /* Increase line counter */
            f->line_count++;
         }
      }
   }

   /* Section file? */
   if (f->type == FILE_TYPE_SECTION) {
      /* Read all lines until end of line and store result in data array */
      while (file_data_get_next(f));
   }
}

/* \fn file_data_free(f, fp)
 * Free file data for current file structure
 * \param[in] f file structure for current file
 */
void file_data_free(struct file_t *f) {
   /* Loop */
   int i = 0;

   /* Loop through all config settings */
   for (i = 0; f->fd[i].html != NULL; i++) {
      /* Object allocated? */
      if (f->fd[i].current) {
         /* Free value */
         free(f->fd[i].current);
         /* Set to zero */
         f->fd[i].current = NULL;
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

   file_data_free(f);

   if (f->line) {
      for (i = 0; i < f->line_count; i++) {
         if (f->line[i]) {
            free(f->line[i]);
            f->line[i] = NULL;
         }
      }
      free(f->line);
      f->line = NULL;
   }

   free(f->name);
   f->name = NULL;

}

/* \fn file_data_read(f, fp)
 * Read data from file into structure
 * \param[in] f file structure for current file
 * \param[in] entry values
 */
void file_data_read(struct file_t *f, char **entry) {
   /* Loop for elements */
   int i = 0;
   /* Loop for config settings */
   int j = 0;

   /* Per line config? */
   if (f->type == FILE_TYPE_LINE) {
      file_data_free(f);

      /* Loop through all entries */
      for (i = 0; entry[i] != NULL; i++) {
         /* Loop through all config settings */
         for (j = 0; f->fd[j].html != NULL; j++) {
            /* Position found? */
            if (f->fd[j].index == i) {
	       /* Copy value */
               f->fd[j].current = strdup(argument_get_part(entry, i));
	    }
	 }
      }
   } else {
      /* Loop through all config settings */
      for (j = 0; f->fd[j].html != NULL; j++) {
         /* Directive found? */
         if (!strcmp(f->fd[j].directive, argument_get_part(entry, 0))) {
            /* Copy value */
            f->fd[j].current = strdup(argument_get_part(entry, 1));
	    /* Set line */
	    f->fd[j].line = f->line_current;
         }
      }
   }
}

/* \fn file_data_get_next(f)
 * Read next file structure  from file
 * \param[in] f file structure for current file
 */
int file_data_get_next(struct file_t *f) {
   /* Current line data */
   char **entry = NULL;

   /* Not first search */
   if (f->line_search) {
      f->line_current++;
   }

   /* Skip comment lines */
   while (f->line && 
          f->line_current < f->line_count &&
	  f->line[f->line_current] &&
          f->line[f->line_current][0] == '#') {
        /* Increase counter */
        f->line_current++;
   }

   /* Current line within index? */
   if (f->line_current < f->line_count) {
      /* Parse current line */
      entry = argument_parse(f->line[f->line_current], f->separator);
      /* Parse successful? */
      if (entry) {
         /* Get line into ini structure */
         file_data_read(f, entry);
         /* Free entry */
         free(entry);
      }
      /* Increase search counter */
      f->line_search++;
      /* Return success */
      return 1;
   } else {
      return 0;
   }
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

   /* Try to open file for writing */
   fp = fopen(f->name, "w");
   /* Successful? */
   if (fp) {
      /* Loop through all lines */
      for (i = 0; i < f->line_count; i++) {
         fprintf(fp, "%s\n", f->line[i]);
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
      free(f->line[f->line_current]);
      /* Allocate data for new line */
      f->line[f->line_current] = (char*)malloc(strlen(line) + 1);
      /* Reset data */
      memset(f->line[f->line_current], 0, strlen(line) + 1);
      /* Copy data to index */
      strcpy(f->line[f->line_current], line);
   /* Del line? */        
   } else if (mode == FILE_LINE_DEL) {
      /* Free current line */
      free(f->line[f->line_current]);
      /* Copy every element after del index to its previous position */
      for (i = f->line_current; i < f->line_count - 1; i++) {
         /* Just set pointer */              
         f->line[i] = f->line[i + 1];
      }
      /* Decrease line counter */
      f->line_count--;
      /* Reallocate new index array */
      f->line = (char**)realloc(f->line,
                                sizeof(char *) *
                                (f->line_count + 1));
   /* Add line? */         
   } else if (mode == FILE_LINE_ADD) {
      /* Increase line counter */
      f->line_count++;
      /* Reallocate new index array */
      f->line = (char**)realloc(f->line,
                                sizeof(char *) *
                                (f->line_count + 1));
      /* Move pointers */
      for (i = f->line_count - 1; i > f->line_current; i--) {
         f->line[i] = f->line[i - 1];
      }
      /* Allocate space for new line */
      f->line[f->line_current] = (char*)malloc(strlen(line) + 1);
      /* Reset data */
      memset(f->line[f->line_current], strlen(line) + 1, 0);
      /* Copy data */
      strcpy(f->line[f->line_current], line);
   }
}

/* \fn file_data_valid(f)
 * Get valid entry for a config setting
 */
char *file_data_valid(struct file_data_t *fd) {
   /* Valid values for checkbox */
   char **valid_values = NULL;
   /* Current valid value */
   char *current = NULL;

   if (fd->type == FILE_DATA_TYPE_CHECKBOX) {
      /* Valid values */
      valid_values = argument_parse(fd->valid, "|");
      if (strcmp(variable_get(fd->html), "") &&
          !strcmp(valid_values[0], variable_get(fd->html))) {
         current = strdup(valid_values[0]);
      } else {
         current = strdup(valid_values[1]);
      }
      argument_free(valid_values);
   } else if (fd->type != FILE_DATA_TYPE_INTERNAL) {
      if (fd->valid) {
	 if (fd->flags & FILE_DATA_FLAG_DONTFILL &&
	     !strcmp(variable_get(fd->html), "")) {
            current = strdup(fd->current);
	 } else if (match(variable_get(fd->html), fd->valid)) {
            current = strdup(variable_get(fd->html));
            if (fd->flags & FILE_DATA_FLAG_CRYPT) {
               current = strdup(crypt(current, "OM"));
            }
         } else {
            variable_error_set(fd->html, OWI_SYNTAX_INVALID);
            variable_set("error", OWI_SYNTAX_INVALID);
         }
      } else {
         current = strdup(variable_get(fd->html));
      }
   }

   /* Return current value */
   return current;
}

/* \fn file_data_update(f)
 * Write current record to line array
 * \param[in] f file structure
 */
void file_data_update(struct file_t *f) {
   /* Loop for line */
   int i = 0;
   /* Loop for config settings */
   int j = 0;
   /* Current entry */
   char *current = NULL;
   /* Entry */
   char **entry = NULL;
   /* Line */
   char *line = NULL;

   /* Per line config? */
   if (f->type == FILE_TYPE_LINE) {
      /* Parse current line */
      entry = argument_parse(f->line[f->line_current], f->separator);
      /* Get line into ini structure */
      file_data_read(f, entry);
      /* Loop through all available config settings */
      for (j = 0; f->fd[j].html != NULL; j++) {
         /* Allow updates for this column? */
	 if (f->fd[j].flags & FILE_DATA_FLAG_UPDATE) {
            /* Get current valid value */
            f->fd[j].current = file_data_valid(&(f->fd[j]));
         }	    
      }
      /* Conver to line for writing */
      line = file_data_write(f);
      /* Write result to current line */
      file_action(f,
                  FILE_LINE_SET,
                  "%s",
                  line);
      /* Free result from line */		  
      free(line);
      /* Loop through all available config settings 
       * and write old config values back to current
       * so user will see wrong syntax for his input */
      for (j = 0; f->fd[j].html != NULL; j++) {
         /* Allow updates for this column? */
         if (f->fd[j].flags & FILE_DATA_FLAG_UPDATE) {
            /* Get current valid value */
            f->fd[j].current = strdup(variable_get(f->fd[j].html));
         }
      }
   } else {
      /* Loop through all config settings */
      for (j = 0; f->fd[j].html != NULL; j++) {
         /* Set current line */
         f->line_current = f->fd[j].line;

         /* Set current config for display too - this non validated value
	  * will not be written to config file (only displayed) */
	 f->fd[j].current = strdup(variable_get(f->fd[j].html));

         /* Remove empty variable? */
         if (f->fd[j].flags & FILE_DATA_FLAG_SKIP_EMPTY &&
	     !strcmp(variable_get(f->fd[j].html), "") &&
	     f->fd[j].line != -1) {
	    /* Set current line */
	    f->line_current = f->fd[j].line;
            /* Remove line */
            file_action(f, FILE_LINE_DEL, NULL);
            /* Update line index */
            for (i = 0; f->fd[i].html != NULL; i++) {
               if (f->fd[i].line > f->line_current) {
                  f->fd[i].line--;
               }
            }
         /* Set current line? */
         } else if (f->fd[j].line != -1) {
            /* Get current valid value */
            current = file_data_valid(&(f->fd[j]));
	    /* Update current line */
            file_action(f,
	                FILE_LINE_SET,
			"%s%s%s",
			f->fd[j].directive,
			f->separator,
                        current);
         /* Not found in config file? */
         } else if (!(f->fd[j].flags & FILE_DATA_FLAG_SKIP_EMPTY) &&
	            strcmp(variable_get(f->fd[j].html), "")) {
            /* Get current valid value */
            current = file_data_valid(&(f->fd[j]));
            /* Add at last position */
            f->line_current = f->line_count;
	    /* Add line line */
            file_action(f,
	                FILE_LINE_ADD,
			"%s%s%s",
			f->fd[j].directive,
			f->separator,
                        current);
         }
      }
   }
}

/* \fn file_data_update(f, id)
 * Find line with correct id
 * \param[in] f file structure
 * \param[in] id id for file data
 * \return 1 when found or 0 when not found
 */
int file_data_find(struct file_t *f, char *id) {
   /* Index for attributes / file structure */
   int index_id = -1;
   /* Entry found? */
   int found = 0;
   /* Loop */
   int i;

   /* Find correct index */
   for (i = 0; f->fd[i].html != NULL; i++) {
      if (f->fd[i].flags & FILE_DATA_FLAG_ID) {
         index_id = i;
         break;
      }
   }

   /* Index found? */
   if (index_id != -1) {
      /* Reset to first line */
      f->line_current = 0;

      /* Loop through all data entries */
      while (file_data_get_next(f)) {
         /* Found? */
         if (!strcmp(f->fd[index_id].current, id)) {
            /* Mark as found */
            found = 1;
            /* Break loop because we're now at correct position */
            break;
         }
      }
   }

   return found;
}

/* \fn file_data_write(f)
 * Convert file structure to one single line for output
 */
char *file_data_write(struct file_t *f) {
   /* Index */
   int i;
   /* New line */
   char *line = NULL;
   /* Line length */
   int line_length = 0;
   /* Current data */
   char *current = NULL;

   /* Loop through all settings */
   for (i = 0; f->fd[i].html != NULL; i++) {
      /* Set current data */
      current = f->fd[i].current;
      /* Not defined? */
      if (!current) {
         /* Just set empty string */
         current = "";
      }

      /* Increase line lenght */
      line_length += strlen(current);

      /* Need separator? */
      if (f->fd[i + 1].html != NULL) {
         line_length++;
      }

      /* Allocate space for line */
      line = realloc(line, line_length + 1);
      /* Enough space? */
      if (line) {
         /* First realloc? */
         if (!i) {
            /* Clear memory first */
            memset(line, 0, line_length + 1);
         }
         strcat(line, current);
         /* Need separator? */
         if (f->fd[i + 1].html != NULL) {
            strcat(line, f->separator);
         }
      }
   }

   /* Return result */
   return line;
}

/* \fn proc_open(f, command)
 * \param[in] f file structure for current file
 * \param[in] command will be executed
 */
void proc_open(struct file_t *f, char *command) {
   /* File handle */
   FILE *fp = NULL;

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
