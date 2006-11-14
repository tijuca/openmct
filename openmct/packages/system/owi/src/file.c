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

/* \fn file_read(filename)
 * Read file into memory (char**) 
 * \param[in] filename file that will be read into memory 
 * \return Amount of lines that have been read
 */
int file_read(char *filename) {
   /* File handle */
   FILE *fp = NULL;
   /* Each line from file */
   char line[FILE_MAXLINE];
   /* Line counter */
   int line_counter = 0;

   /* Try to open file */
   fp = fopen(filename, "r");
   /* Successful? */
   if (fp) {
      while (fgets(line, sizeof(line), fp)) {
         /* Strip \n from line */
         line[strlen(line) - 1] = 0;
         /* Reallocate array */
         file_line = (char**)realloc(file_line,
                                                    sizeof(char *) *
                                                    (line_counter + 1));
         file_line[line_counter] = (char*)malloc(strlen(line) + 1);
         /* Add to data */
         strcpy(file_line[line_counter], line);
         /* Increase line counter */
         line_counter++;
      }
      file_line_counter = line_counter;
      /* Close file */
      fclose(fp);
   } else {
      /* Print error message on stderr */
      fprintf(stderr, "[*] can't open file (%s)\n", filename);
   }

   /* Return amount of lines */
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

   /* Try to open file for writing */
   fp = fopen(filename, "w");
   /* Successful? */
   if (fp) {
      /* Print data to file */
      file_print(fp);
      /* Close file */
      fclose(fp);
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

void passwd_del(char *username) {
   /* Index counter */
   int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Username matches? */
      if (!strcasecmp(username, passwd[0])) {
         /* Delete username */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free passwd entry */
      argument_free(passwd);
   }
}

void nfs_update(char *mountpoint, char *options) {
   /* Index counter */
   int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **nfs = argument_parse(file_line_get(i), "    ");
      /* Matches? */
      if (!strcasecmp(nfs[0], mountpoint)) {
         file_line_action(FILE_LINE_SET, i,
                          "%s %s", nfs[0], options);
      }
      /* Free nfs entry */
      argument_free(nfs);
   }
}

void nfs_del(char *mountpoint) {
   /* Index counter */
   int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **nfs = argument_parse(file_line_get(i), "    ");
      /* Matches? */
      if (!strcasecmp(nfs[0], mountpoint)) {
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
   }
}

void interfaces_update(char *interface, char *ip, char *netmask, char *broadcast, char *gateway) {
   /* Index counter */
   int i;
   /* Correct section? */
   int section = 0;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **line = argument_parse(file_line_get(i), "   ");

      /* At least two parameter found? */
      if (line && line[0] && line[1]) {
         /* New interface start? */
         if (!strcasecmp(line[0], "auto")) {
            /* Interfaces matches? */
            if (!strcasecmp(line[1], interface)) {
               /* We are in the correct section */
               section = 1;
            } else {
               /* We are not in the correct section */
               section = 0;
            }
         /* Else correct section? */
         } else if (section) {
            /* Prefix padding for variable settings */
            int prefix = 0;

            if (!strcasecmp(line[0], "address")) {
               /* get padding for address options */
               prefix = file_get_pad(file_line_get(i), "address");
               /* Set new ip address */
               file_line_action(FILE_LINE_SET, i,
                                "%*s %s %s", prefix, "address", ip, prefix);
            } else if (!strcasecmp(line[0], "netmask")) {
               /* get padding for address options */
               prefix = file_get_pad(file_line_get(i), "netmask");
               /* Set new netmask address */
               file_line_action(FILE_LINE_SET, i,
                                "%*s %s %s", prefix, "netmask", netmask, prefix);
            } else if (!strcasecmp(line[0], "gateway")) {
               /* get padding for address options */
               prefix = file_get_pad(file_line_get(i), "gateway");
               /* Set new gateway address */
               file_line_action(FILE_LINE_SET, i,
                                "%*s %s %s", prefix, "gateway", gateway, prefix);
            }
         }
      }

      /* Free line */
      argument_free(line);
   }
}
