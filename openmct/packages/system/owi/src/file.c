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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "includes/file.h"
#include "includes/argument.h"

char **file_line= NULL;
int file_line_counter = 0;

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
         memset(file_line[line_index], strlen(line) + 1, 0);
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
