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

char **file_line = NULL;

int file_line_counter = 0;

int file_section_start = -1;
int file_section_stop = -1;

/* \fn file_open(filename)
 * Read file into memory (char**) 
 * \param[in] filename file that will be read into memory 
 * \return Amount of lines that have been read
 */
int file_open(char *filename) {
   /* File handle */
   FILE *fp = NULL;
   /* Line counter - set -1 on default for error */
   int line_counter = -1;
   /* Real filename */
   char *real_filename = (char*)malloc(strlen(filename) + strlen(FILE_BASE) + 1);

   /* Malloc ok? */
   if (real_filename) {
      /* Write real filename into variable */
      sprintf(real_filename, "%s%s", FILE_BASE, filename);
      /* Try to open file */
      fp = fopen(real_filename, "r");
      /* Successful? */
      if (fp) {
         /* Read data */
         line_counter = file_read(fp);
         /* Close file */
         fclose(fp);
      }
      /* Free memory for filename */
      free(real_filename);
   }
   /* Return amount of lines */
   return line_counter;
}

/* \fn proc_read(char *command)
 * Execute program and parses output
 * \param[in] command command that will be executed
 * \return Amount of lines that have been read
 */
int proc_open(char *command) {
   /* File handle */
   FILE *fp = NULL;
   /* Line counter */
   int line_counter;

   /* Try to execute */
   fp = popen(command, "r");
   /* Successful? */
   if (fp) {
      /* Read data */
      line_counter = file_read(fp);
      /* Close file */
      pclose(fp);
   } else {
      /* Set error */
      line_counter = -1;
   }

   /* Return amount of lines */
   return line_counter;
}

/* \fn file_read(fp)
 * \param[in] fp file pointer
 */
int file_read(FILE *fp) {
   /* Each line from file */
   char line[FILE_MAXLINE];
   /* Line counter */
   int line_counter = 0;

   /* Loop through file */
   while (fgets(line, sizeof(line), fp)) {
      /* Strip \n from line */
      line[strlen(line) - 1] = 0;
      /* Reallocate array */
      file_line = (char**)realloc(file_line,
                                  sizeof(char *) * (line_counter + 1));
      file_line[line_counter] = (char*)malloc(strlen(line) + 1);
      /* Add to data */
      strcpy(file_line[line_counter], line);
      /* Increase line counter */
      line_counter++;
   }
   file_line_counter = line_counter;

   /* Return bytes */
   return line_counter;
}

/* \fn file_line(line_number)
 * \param[in] line_number get data from this line
 * \return Pointer to data for this line or NULL on failure
 */
char *file_line_get(int line_number) {
   /* line_number is valid? */
   if (line_number >= 0 && line_number < file_line_counter) {
      /* Return pointer for this line */
      return file_line[line_number];
   } else {
      /* Index invalid or not found? Return NULL */
      return NULL;
   }
}

/* \fn file_print(fp)
 * Print whole file on file handle fp
 * \param[in] fp file handle
 */
void file_print(FILE *fp) {
   /* Index counter */
   int i;
   /* Loop through all lines */
   for (i = 0; i < file_line_counter; i++) {
      fprintf(fp, "%s\n", file_line_get(i));
   }
}

/* \fn file_save(filename)
 * Save lines to this file
 * \param[in] filename data will be written to this file
 */
void file_save(char *filename) {
   /* File handle */
   FILE *fp = NULL;
   /* Real filename */
   char *real_filename = (char*)malloc(strlen(filename) + strlen(FILE_BASE) + 1);

   /* Malloc ok? */
   if (real_filename) {
      /* Write real filename into variable */
      sprintf(real_filename, "%s%s", FILE_BASE, filename);
      /* Try to open file for writing */
      fp = fopen(real_filename, "w");
      /* Successful? */
      if (fp) {
         /* Print data to file */
         file_print(fp);
         /* Close file */
         fclose(fp);
      }
      /* Free filename */
      free(real_filename);
   }
}

/* \fn file_free()
 * Free all lines and reset data
 */
void file_free() {
   /* Index counter */
   int i;
   /* Loop through all lines */
   for (i = 0; i < file_line_counter; i++) {
      /* Free line */
      free(file_line[i]);
   }
   /* Free index for lines */
   free(file_line);
   /* Set line counter to zero */
   file_line_counter = 0;
   /* Set pointer to NULL */
   file_line = NULL;
}

/* \fn file_line_action
 * Add/Set/Del a line from line array
 * \param[in] mode Delete, Add or Set line
 * \param[in] line_index Delete or Set this line or add a line at this index position
 * \param[in] format format string for line
 * \param[in] ... Arguments on stack for *printf
 */
void file_line_action(int mode, int line_index, char *format, ...) {
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
      /* Valid index? */
      if (line_index >= 0 && line_index < file_line_counter) {
         /* Free old data */
         free(file_line[line_index]);
         /* Allocate data for new line */
         file_line[line_index] = (char*)malloc(strlen(line) + 1);
         /* Reset data */
         memset(file_line[line_index], 0, strlen(line) + 1);
         /* Copy data to index */
         strcpy(file_line[line_index], line);
      }
   /* Del line? */        
   } else if (mode == FILE_LINE_DEL) {
      /* Valid index? */
      if (line_index >= 0 && line_index < file_line_counter) {
         /* Free current line */
         free(file_line[line_index]);
         /* Copy every element after del index to its previous position */
         for (i = line_index; i < file_line_counter - 1; i++) {
            /* Just set pointer */              
            file_line[i] = file_line[i + 1];
         }
         /* Decrease line counter */
         file_line_counter--;
         /* Reallocate new index array */
         file_line = (char**)realloc(file_line,
                                     sizeof(char *) *
                                     (file_line_counter + 1));
      }         
   /* Add line? */         
   } else if (mode == FILE_LINE_ADD) {
      /* Valid index? */
      if (line_index >= 0 && line_index <= file_line_counter) {
         /* Increase line counter */
         file_line_counter++;
         /* Reallocate new index array */
         file_line = (char**)realloc(file_line,
                                     sizeof(char *) *
                                     (file_line_counter + 1));
         /* Move pointers */
         for (i = file_line_counter - 1; i > line_index; i--) {
            file_line[i] = file_line[i - 1];
         }
         /* Allocate space for new line */
         file_line[line_index] =
            (char*)malloc(strlen(line) + 1);
         /* Reset data */
         memset(file_line[line_index],
                strlen(line) + 1,
                0);
         /* Copy data */
         strcpy(file_line[line_index],
                line);
      }
   }
}

/* \fn file_get_pad(string, match)
 * \param[in] string search string (in most cases the whole line)
 * \param[in] match match string (in most variable name)
 * \return padding lengh for *printf functions
 */
int file_get_pad(char *string, char *match) {
   return (strstr(string, match) - string) + strlen(match);
}

/* \fn proc_read_line(command, index)
 * \param[in] proc command to execute
 * \param[in] index return this line
 * \return string
 */
char *proc_read_line(char *command, int index) {
   int lines = proc_open(command);
   char *line = NULL;

   if (lines >= 0) {
      if (index <= lines) {
         line = (char*)malloc(strlen(file_line_get(index)) + 1);
	 if (line) {
            strcpy(line, file_line_get(index));
	 }
      }
      file_free();
   }

   return line;
}

/* \fn file_read_line(command, index)
 * \param[in] file to be read
 * \param[in] index return this line
 * \return string
 */
char *file_read_line(char *file, int index) {
   int lines = file_open(file);
   char *line = NULL;

   if (lines >= 0) {
      if (index <= lines) {
         line = (char*)malloc(strlen(file_line_get(index)) + 1);
         if (line) {
            strcpy(line, file_line_get(index));
         }
      }
      file_free();
   }

   return line;
}

/* \fn file_data_read(ini) 
 * Read contents from file into ini structure 
 * \param[in] ini file ini structure
 */
void file_data_read(struct file_data_t *ini, char *separator) {
   /* Loop */
   int i = 0;
   /* Loop */
   int j = 0;
   /* Config line */
   char **entry = NULL;
   /* Start file parsing at position start */
   int start = file_section_start >= 0 ? file_section_start : 0;
   /* Stop file parsing at position stop */
   int stop = file_section_stop >= 0 ? file_section_stop : file_line_counter;

   for (j = 0; ini[j].html != NULL; j++) {
      /* Reset to zero per default */
      ini[j].current = NULL;
      for (i = start; i < file_line_counter && i < stop; i++) {
         /* Parse line */
         entry = argument_parse(file_line_get(i), separator);
         /* Directive found? */
         if (entry && entry[0] && !strcmp(entry[0], ini[j].directive)) {
            /* HTML variable set (during update process)? */
            if (strcmp(variable_get(ini[j].html), "")) {
               /* Set current value */
               ini[j].current = variable_get(ini[j].html);
            /* Just display from config? */
            } else {
               /* Set current value */
               ini[j].current = strdup(argument_get_part(entry, 1));
            }
            /* Set line */
            ini[j].position = i;
         }
         /* Free configuration line */
         argument_free(entry);
      }
   }
}

/* \fn file_data_detail(ini)
 * \param[in] ini file ini structure
 */
void file_data_detail(struct file_data_t *ini, char **entry) {
   int j;

   for (j = 0; ini[j].html != NULL; j++) {
      /* HTML variable set (during update process)? */
      if (strcmp(variable_get(ini[j].html), "")) {
         /* Set current value */
         ini[j].current = variable_get(ini[j].html);
      /* Just display from config? */
      } else {
         /* Set current value */
         ini[j].current = strdup(argument_get_part(entry, ini[j].index));
      }
   }

}

/* \fn file_data_update(ini)
 * Update contents in file from ini structure
 * \param[in] ini file ini structure
 */
void file_data_update(struct file_data_t *ini, char *separator) {
   /* Loop */
   int i = 0;
   /* Loop */
   int j = 0;
   /* Variable empty? */
   int empty = 0;

   /* Loop through all directives */
   for (i = 0; ini[i].html != NULL; i++) {
      /* Skip empty config values? */
      if (ini[i].flags & FILE_DATA_FLAG_SKIP_EMPTY &&
          !strcmp(variable_get(ini[i].html), "")) {
         empty = 1;
      } else {
         empty = 0;
      }
      /* Config setting found in config file? */
      if (ini[i].position != -1) {
         /* Empty and remove line? */
	 if (empty) {
	    /* Remove line */
	    file_line_action(FILE_LINE_DEL, ini[i].position, NULL);
	    /* Remove variable so it will not be displayed */
	    ini[i].current = NULL;
	    /* Decrease section stop */
	    file_section_stop--;
	    /* Decrease position pointers */
	    for (j = 0; ini[j].html != NULL; j++) {
               if (ini[j].position > ini[i].position) {
                  ini[j].position--;
	       }
	    }
	 } else {
            /* Set in file */
            file_data_update_detail(&ini[i], ini[i].position, separator);
	 }
      /* Config setting not found? */
      } else if (!empty) {
         /* Update current line */
         file_data_update_detail(&ini[i],
	                         file_section_stop != -1 ? file_section_stop : file_line_counter,
				 separator);
      }
   }
}

/* \fn file_data_update_detail(ini, line_index)
 * Update one config setting
 * \param[in] ini file ini structure
 * \param[in] line_index index for current setting in file
 */
void file_data_update_detail(struct file_data_t *ini, int line_index, char *separator) {
   /* Current value */
   char *value = NULL;
   /* Valid values for config value */
   char **valid_values = NULL;
   /* Default file action */
   int action = FILE_LINE_SET;

   if (ini->type == FILE_DATA_TYPE_CHECKBOX) {
      /* Valid values */
      valid_values = argument_parse(ini->valid, "|");
      if (strcmp(variable_get(ini->html), "") &&
          !strcmp(valid_values[0], variable_get(ini->html))) {
         value = valid_values[0];
      } else {
         value = valid_values[1];
      }
   } else {
      if ( (ini->valid && match(variable_get(ini->html), ini->valid)) ||
           !(ini->valid)) {
         value = variable_get(ini->html);
      }
   }

   /* Set current value for display -
    * can be different from file because of errors */
   ini->current = strdup(variable_get(ini->html));

   /* Last line in file or block? */
   if (line_index == file_line_counter ||
       line_index == file_section_stop) {
      /* Its and add request */
      action = FILE_LINE_ADD;
      /* Increase stop section */
      file_section_stop++;
   }

   if (value) {
      /* Write new config line */
      file_line_action(action, line_index,
                       "%s%s%s",
                       ini->directive,
		       separator,
                       variable_get(ini->html));
   } else {
      variable_error_set(ini->html, OWI_SYNTAX_INVALID);
      variable_set("error", OWI_ERROR_INVALID);
   }
}

/* \fn file_data_update_column(column, line_index)
 * Update one column line
 * \param[in] column file column structure
 */
void file_data_update_column(struct file_data_t *column, int line_index, char **entry) {
   /* Index counter */
   int i;
   /* Index counter */
   int j;
   /* Error variable */
   char *error = NULL;
   /* Column found? */
   int column_found = 0;
   /* Current value */
   char *p = NULL;
   /* Data */
   char *data = NULL;
   /* Length for data memory */
   int data_length = 0;
   /* Separator needed? */
   int separator = 0;

   /* Verify syntax of columns */
   for (i = 0; column[i].html != NULL; i++) {
      if ( !(column[i].flags & FILE_DATA_FLAG_DONTFILL) &&
            column[i].type != FILE_DATA_TYPE_CHECKBOX &&
            column[i].type != FILE_DATA_TYPE_INTERNAL &&
           (!match(variable_get(column[i].html), column[i].valid)) ) {
         error = OWI_ERROR_INVALID;
	 variable_error_set(column[i].html, OWI_SYNTAX_INVALID);
	 variable_set("error", OWI_ERROR_INVALID);
      }
   }

   /* Error during update? */
   if (error) {

   } else {
      /* Lookup internal details */
      for (j = 0; column[j].html != NULL; j++) {
         if (column[j].type == FILE_DATA_TYPE_INTERNAL) {
            for (i = 0; column[i].html != NULL; i++) {
               if (!strcmp(column[i].name, column[j].name)) {
	           char **valid_names = argument_parse(column[i].valid, "|");
                   if (!strcmp(valid_names[0], variable_get(column[i].html))) {
                      variable_set(column[j].html, column[j].standard);
		   }
		   free(valid_names);
	       }
	    }
	 }
      }

      /* Loop through all entries */
      for (i = 0; entry[i] != NULL; i++) {
         /* Mark column as not found per default */
         column_found = 0;
	 /* Check for a config variable for this index position */
         for (j = 0; column[j].html != NULL; j++) {
	    /* Variable found? */
            if (i == column[j].index && strcmp(variable_get(column[j].html), "")) {
	       column_found = 1;
               p = variable_get(column[j].html);
	       if (column[j].flags & FILE_DATA_FLAG_CRYPT) {
	          p = crypt(p, "OM");
	       }
	    }
	 }
         /* Not found or value not set via http? */
	 if (!column_found || !p) {
            /* Use value from config file */
	    p = argument_get_part(entry, i);
	 }
	 /* Need separator? */
	 if (entry[i + 1] != NULL) {
            separator = 1;
	 } else {
            separator = 0;
	 }
	 /* Calculate string size */
         data_length += strlen(p) + separator;
	 /* Allocate memory for line */
	 data = realloc(data, data_length + 1);
	 /* find? */
	 if (data) {
	    /* First allocate? */
	    if (data_length == strlen(p) + separator) {
	        /* Reset buffer */
                memset(data, 0, data_length + 1);
	    }
	    /* Copy data */
	    strcat(data, p);
	    if (separator) {
               strcat(data, ":");
	    }
	 }
      }
      /* Write new config line */
      file_line_action(FILE_LINE_SET, line_index,
                       "%s",
                       data);
      
      /* Free line */
      free(data);
   }
}
