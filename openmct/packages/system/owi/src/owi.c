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
#include <sys/resource.h>
#include "includes/argument.h"
#include "includes/modules.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/misc.h"
#include "includes/owi.h"

void owi_header(char *title) {
   /* Loop counter */
   int i = 0;

   printf("Content-Type: text/html\n\n");
   fflush(stdout);
 
   printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
          "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
          "<head>\n"
          "<title>%s</title>\n"
          "<link rel=\"stylesheet\" type=\"text/css\" href=\"../css/openmct.css\" />\n"
          "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" />\n"
          "</head>\n"
          "<body>\n"
	  "<table class=\"main\">\n"
	  "<tr>\n"
	  "<td class=\"logo\"><h1>OpenMCT</h1></td>\n"
	  "<td class=\"description\"><h2>Administration Interface</h2></td>\n"
	  "<td class=\"domain\" align=\"right\">domainname</td>\n"
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"navigation\">\n"
	  "<table>\n",
	  title);
   for (i = 0; modules[i].description != NULL; i++) {
      printf("<tr><td>\n");
      if (modules[i].main) {
         printf("<a href=\"%s?module=%s\">%s</a>\n",
                getenv("SCRIPT_NAME"),
	        modules[i].name,
	        modules[i].description);
      } else {
         printf("%s", modules[i].description);
      }
      printf("</td></tr>\n");
   }
   printf("</table>\n"
          "</td>\n"
          "<td class=\"content\">\n");
}

void owi_footer() {
   printf("</td>\n"
          "</tr>\n"
	  "</table>\n"
	  "<table class=\"footer\">\n"
	  "<tr>\n"
	  "<td>OpenMCT. All rights reserved.</td>\n"
	  "</tr>\n"
	  "</table>\n"
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
   char *module = NULL;

   owi_request();

   module = variable_get("module");

   if (!strcmp(module, "")) {
      variable_set("module", "sysinfo");
      module =  variable_get("module");
   }

   for (i = 0; modules[i].name != NULL; i++) {
      if (!strcmp(module, modules[i].name)) {
         return modules[i].main(argc, argv);
      }
   }

   printf("Content-Type: text/html\n\nModule nicht gefunden\n");

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
   /* Parameter that are passed via CGI */
   char **parameter = NULL;

   /* GET request? */
   if (request_method && !strcasecmp(request_method, "GET")) {
      /* Parse parameters */
      parameter = argument_parse(getenv("QUERY_STRING"), ARGUMENT_SEPERATOR_CGI);
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
            /* Parse parameters */
            parameter = argument_parse(post, ARGUMENT_SEPERATOR_CGI);
         }
         /* Free post data */
         free(post);
       }
    }

    /* HTTP parameter found? */
    if (parameter) {
       /* Loop through parameters */
       for (i = 0; parameter[i] != NULL; i++) {
          /* Get position of = sign */
          equal = strchr(parameter[i], '=');
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
             variable_set(parameter[i], equal + 1);
          }
          /* Set equal sign */
          *equal = '=';
       }
       /* Free parameters via HTTP */
       free(parameter);
    }
}
