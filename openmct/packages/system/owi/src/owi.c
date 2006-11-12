/* -*- mode: C; c-file-style: "gnu" -*- */
/* owi.c  main module
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
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "includes/argument.h"
#include "includes/modules.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/misc.h"
#include "includes/owi.h"

void owi_header(char *title) {
   printf("Content-Type: text/html\n\n");
        
   printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"" 
          "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
          "<html xmlns=\"http://www.w3.org/1999/xhtml\">"
          "<head>\n"
          "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n"
          "<title>%s</title>\n"
          "<link rel=\"stylesheet\" type=\"text/css\" href=\"css/openmct.css\" />\n"
          "</head>\n"
          "<body>\n", title);
}

void owi_footer() {
   printf("</body>\n"
          "</html>\n");
}

void owi_image(char *image, char *title, char *alt) {
   printf("<img src=\"%s\" title=\"%s\" alt=\"%s\" />", image, title, alt);
}

void owi_headline(int size, char *headline) {
   printf("<h%d>%s</h%d>\n", size, headline, size);
}

int main(int argc, char **argv) {
   int i;
   char *p = strrchr(argv[0], '/');
   char *module = NULL;

   if (p) {
      module = p + 1;
   } else {
      module = p;
   }

   owi_request();

   for (i = 0; modules[i].name != NULL; i++) {
      if (!strcmp(module, modules[i].name)) {
         return modules[i].main(argc, argv);
      }
   }

   return 0;
}

/* \fn owi_request()
 * Parses request data from HTTP client (GET or POST request)
 */
void owi_request() {
   /* Get request method */
   char *request_method = getenv("REQUEST_METHOD");
   /* Get content length */
   char *content_length = getenv("CONTENT_LENGTH");
   /* Post data */
   char *post = NULL;
   /* Content lenght */
   unsigned int length;
   /* Counter for loop */
   unsigned int i;
   /* Position of equal sign */
   char *equal = NULL;
   /* Pointer for handling variable set */
   char *p, *o;

   /* GET request? */
   if (request_method && !strcasecmp(request_method, "GET")) {
      /* Parse arguments */
      argument = argument_parse(getenv("QUERY_STRING"), ARGUMENT_SEPERATOR_CGI);
   /* POST request and content_length set? */
   } else if (request_method && !strcasecmp(request_method, "POST") && content_length) {
      /* convert string to integer */
      length = atoi(content_length);
      /* Allocate memory for post data */
      post = (char*)malloc(length + 1);
      /* Allocate ok? */
      if (post) {
         /* Reset data to zero */
         memset(post, 0, length + 1);
         /* Read post data */
         if (read(0, post, length)) {
            /* Parse arguments */
            argument = argument_parse(post, ARGUMENT_SEPERATOR_CGI);
         }
         /* Free post data */
         free(post);
       }
    }

    /* HTTP parameter found? */
    if (argument) {
       /* Loop through arguments */
       for (i = 0; argument[i] != NULL; i++) {
          /* Get position of = sign */
          equal = strchr(argument[i], '=');
          /* Found? */
          if (equal) {
             /* Set to 0 so string will terminate here */
             *equal = 0;
             /* Set o and p to first sign after = sign */
             o = p = equal + 1;
             /* Loop until string end reached */
             while (*p) {
                /* % sign found? */
                if (*p == '%') {
                   /* Convert next two bytes to a byte value */
                   *o++ = (char)hex2byte(p + 1, 2);
                   /* Skip next two bytes */
                   p += 3;
                /* + sign found? */
                } else if (*p == '+') {
                   /* Just set a space */
                   *o++ = ' ';
                   /* Increase position by one */
                   p++;
                } else {
                   /* Just copy current value */
                   *o++ = *p++;
                }
             }
             /* Terminate string */
             *o = 0;
             /* define variable name */
             variable_set(argument[i], equal + 1);
          }
          /* Set equal sign */
          *equal = '=';
       }
       /* Free arguments via HTTP */
       free(argument);
    }
}
