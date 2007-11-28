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
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <crypt.h>
#include <sys/resource.h>
#include "includes/language.h"
#include "includes/argument.h"
#include "includes/variable.h"
#include "includes/misc.h"
#include "includes/file.h"
#include "includes/data.h"
#include "includes/owi.h"
#include "includes/user.h"
#include "includes/group.h"
#include "includes/nfs.h"
#include "includes/ftp.h"
#include "includes/process.h"
#include "includes/interface.h"
#include "includes/smb.h"
#include "includes/rc.h"
#include "includes/harddisk.h"
#include "includes/modules.h"

/* \fn owi_header(owi)
 * Display header (the same for every page)
 * \param[in] owi owi settings
 */
void owi_header(struct owi_t *owi) {
   /* Loop counter */
   int i = 0;
   int j = 0;

   /* Print main content for every page */
   printf("Content-Type: text/html\n\n"
          "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
	  "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
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
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"navigation\">\n"
	  "<table>\n",
	  owi->headline);
   /* Display all enabled modules in navigation content */	  
   for (i = 0; modules[i].description != NULL; i++) {
      printf("<tr><td>\n");
      for (j = 0; j < modules[i].level * 5; j++) {
         printf("&nbsp;");
      }
      /* Link available for module? */
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
          "<td class=\"content\">\n"
	  "<br />\n"
          "<table width=\"100%%\">\n"
	  "<tr>\n"
	  "<td class=\"headline_%s\">%s</td>\n"
	  "<td align=\"right\"><a class=\"reload\" href=\"%s?module=%s&amp;id=%s\">Reload</a>\n"
	  "</tr>\n"
	  "</table>\n"
	  "<br />\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
	  "<input type=\"hidden\" name=\"command\" value=\"%s\" />\n"
	  "<input type=\"hidden\" name=\"id\" value=\"%s\" />\n",
          variable_get("module"), owi->headline ? owi->headline: "",
	  getenv("SCRIPT_NAME"),
	  variable_get("module"),
	  variable_get("id"),
          getenv("SCRIPT_NAME"),
          variable_get("module"),
	  variable_get("command"),
	  variable_get("id"));

   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"boxerror\">%s</div>\n",
             variable_get("error"));
   } else if (strcmp(variable_get("info"), "")) {
      printf("<div class=\"boxinfo\">%s</div>\n",
             variable_get("info"));
   }

   printf("<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n"
          "<table class=\"data\" width=\"100%%\">\n");
}

/* \fn owi_footer(owi)
 * Display the footer (the same for every page)
 * \param[in] owi owi settings
 */
void owi_footer(struct owi_t *owi, char *button, ...) {
   /* Dynamic number of arguments */
   va_list list;
   /* Current button */
   char *current;

   printf("</table>\n"
          "<table width=\"100%%\">\n"
          "<tr>\n"
          "<td width=\"100%%\" align=\"right\">");

   /* Start at first element */
   va_start(list, button);

   /* Loop through all parameter */
   for (current = button; current != NULL; current = va_arg(list, char *)) {
      printf("<input type=\"button\" class=\"button\" "
             "onClick=\"javascript:document.forms[0].command.value='%s';"
             "document.forms[0].submit()\" value=\"%s\" />\n",
             current, current);
   }

   printf("</td></tr>\n"
          "</table>\n"
          "</td>\n"
          "</tr>\n"
	  "</table>\n"
	  "</form>\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
	  "<table class=\"footer\">\n"
	  "<tr>\n"
	  "<td>OpenMCT is &copy; 2005 by OpenMCT. All rights reserved.<br />"
	  "<a href=\"http://www.openmct.org\" target=\"_blank\">http://www.openmct.org</a></td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</html>\n");
}

/* \fn owi_error(variable) 
 * Display image if error is set
 * \param[in] variable
 */
void owi_error(char *value) {
   if (value) {
      printf("<img class=\"error\" src=\"/images/icon_warning.png\" alt=\"%s\" title=\"%s\"/>\n",
             value, value);
   }
}

/* \fn owi_table_header(owi)
 * Print table header for current data defintion 
 * \param[in] owi owi settings
 */
void owi_table_header(struct owi_t *owi) {
   /* Loop */
   int i = 0;

   printf("<tr>\n");
   /* Print all header values for table */
   for (i = 0; owi->data[i].type; i++) {
      if (owi->data[i].flags & DATA_FLAG_LIST) {
          printf("<td class=\"list_header header_%s_%s\">%s</td>\n",
                 variable_get("module"), owi->data[i].html, owi->data[i].name);
      }
   }
   /* Display action column? */
   if (owi->flags & OWI_FLAG_ACTION) {
      printf("<td class=\"list_header header_%s_action\">%s</td>\n",
             variable_get("module"), "Action");
   }	     
   /* Start form / external table / scroll area / internal table*/
   printf("</tr>\n");
}

/* \fn owi_data_display(owi, current, flags)
 * Display one value in html (depending on type, flags)
 * \param[in] data current entry
 * \param[in] current current data value
 * \param[in] flags flags for display behavior
 */
void owi_data_display(struct data_t *data, char *current, int flags) {
   /* Loop */
   int i = 0;
   /* Valid values for html elements */
   char **valid_values;

   /* Flags set so its not just display values? */
   if (data->flags & flags) {
      /* Checkbox? */
      if (data->type == DATA_TYPE_CHECKBOX) {
         /* Parse valid values from string */
         valid_values = argument_parse(data->valid, "|");
         /* Print checkbox */
         printf("<input type=\"checkbox\" class=\"checkbox\" "
	        "name=\"%s\" value=\"%s\" %s /> ",
                data->html,
                valid_values[0],
                current && !strcmp(current, valid_values[0]) ?  "checked" : "");
         /* Free valid values */
         free(valid_values);
      /* Selectbox? */
      } else if (data->type == DATA_TYPE_SELECT) {
         /* Parse valid values from string */
         valid_values = argument_parse(data->valid, "|");
   
         /* Print selectbox */
         printf("<select name=\"%s\">\n", data->html);
         /* Loop through values */
         for (i = 0; valid_values[i] != NULL; i++) {
            /* Flag for current value */
            int current_value = 0;
   	 /* Current value? */
   	 if (current && current &&
   	    strcmp(current, valid_values[i])) {
               current_value = 1;
   	 }
            /* Print on stdout */
            printf("<option value=\"%s\" %s>%s</option>\n",
                   valid_values[i],
   		current_value == 0 ? " selected" : "",
   		valid_values[i]);
         }
         printf("</select><br />\n");
         /* Free valid values */
         argument_free(valid_values);
            
      } else if (data->type == DATA_TYPE_TEXT) {
         printf("<input type=\"text\" class=\"%s\" name=\"%s\" value=\"%s\" />",
   	     strcmp(variable_error_get(data->html), "") ? "errortext" : "text",
                data->html,
   	     data->flags & DATA_FLAG_DONT_FILL ? "" : current ? current : "");
         if (strcmp(variable_error_get(data->html), "")) {
            printf("&nbsp;");
   	    owi_error(variable_error_get(data->html));
         } 
         printf("<br />\n");
      } else if (data->type == DATA_TYPE_PASSWORD) {
         printf("<input type=\"password\" class=\"%s\" name=\"%s\" "
	        "value=\"%s\" />",
                strcmp(variable_error_get(data->html), "") ? "errortext" : "text",
                data->html,
                data->flags & DATA_FLAG_DONT_FILL ? "" : current ? current : "");
         if (strcmp(variable_error_get(data->html), "")) {
            printf("&nbsp;");
            owi_error(variable_error_get(data->html));
         }
         printf("<br />\n");
         
      } else if (data->type == DATA_TYPE_TEXTAREA) {
         printf("<textarea name=\"%s\" class=\"%s\" rows=\"4\" cols=\"55\">%s"
	        "</textarea>",
   	        strcmp(variable_error_get(data->html), "") ? "errortext" : "text",
                data->html,
   	     current ? current : "");
         if (strcmp(variable_error_get(data->html), "")) {
            printf("<br />\n");
            owi_error(variable_error_get(data->html));
         }
         printf("<br />\n");
      }
      if (data->description) {
         printf("%s", data->description);
      }
   } else {
      printf("%s<br />\n", current ? current : "");
   }
}

/* \fn owi_detail(owi)
 * Print detail screen for current entry
 * \param[in] owi owi settings
 */
void owi_detail(struct owi_t *owi) {
   /* Loop */
   int i;
   /* Current value */
   char *current = NULL;
   /* Index for unique column */
   int index_id = 0;

   if (owi->flags & OWI_FLAG_ROW) {
      /* Loop through all settings */
      for (i = 0; owi->data[i].type; i++) {
         if (owi->data[i].flags & DATA_FLAG_ID) {
            index_id = i;
	 }
      }
   }


   /* Loop through all settings */
   for (i = 0; owi->data[i].type; i++) {
      /* Display value? */
      if (owi->data[i].type != DATA_TYPE_INTERNAL) {
         printf("<tr>"
                "<td class=\"detail_name\">%s</td>"
                "<td class=\"detail_value detail_value\">",
                owi->data[i].name);
         if (owi->flags & OWI_FLAG_CONFIG) {
            current = FILE_CONFIG_GET(owi->file, owi->data[i].directive);
         } else {
            current = file_value_get(owi->file, index_id, variable_get("id"), i); 
         }
         owi_data_display(&owi->data[i], current, DATA_FLAG_UPDATE);
         printf("</tr>\n");
      }	 
   }
}


void owi_list(struct owi_t *owi) {
   /* Loop */
   int j, i;
   /* id */
   char *id = NULL;

   /* Loop through all lines */
   for (i = 0; i < owi->file->line_count; i++) {

      /* Comment? */
      if (owi->file->line[i]->data[0] == '#')
         continue;
 
      printf("<tr>\n");

      for (j = 0; owi->data[j].type; j++) {
         if (owi->data[j].flags & DATA_FLAG_LIST) {
            printf("<td class=\"list_data list_%s_%s\">%s</td>\n",
	           variable_get("module"), owi->data[j].html, owi->file->line[i]->current[j] ? owi->file->line[i]->current[j] : "");
         }
         if (owi->data[j].flags & DATA_FLAG_ID) {
            id = owi->file->line[i]->current[j];
         }
      }

      /* Print action buttons? */
      if (owi->flags & OWI_FLAG_ACTION) {
         printf("<td class=\"list_data list_%s_%s\">\n"
                "<a href=\"#\" class=\"details\" "
		"onClick=\"javascript:document.forms[0].command.value='%s';"
		"document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n"
                "<a href=\"#\" class=\"delete\" "
		"onClick=\"javascript:document.forms[0].command.value='%s';"
		"document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n"
                "</td>\n"
	        "</tr>\n",
	        variable_get("module"), "action",
                OWI_BUTTON_DETAIL, id, OWI_BUTTON_DETAIL,
                OWI_BUTTON_DELETE, id, OWI_BUTTON_DELETE);
      }		
   }	  
}

/* \fn owi_update(owi)
 * Update settings
 * \param[in] owi owi module
 */
void owi_update(struct owi_t *owi) {
   /* Loop */
   int i;
   /* html value */
   char *html = NULL;
   /* Current value */
   char *current = NULL;
   /* Update in file too? */
   int update = 0;
   /* Index id */
   int index_id = -1;

   /* Row based source? */
   if (owi->flags & OWI_FLAG_ROW) {
      /* Lookup index id */
      for (i = 0; owi->data[i].type; i++) {
         /* Primary key found? */
         if (owi->data[i].flags & DATA_FLAG_ID) {
	    /* Set this index for lookup */
            index_id = i;
	 }
      }
   }

   printf("Content-Type: text/html\n\n");

   /* Per default update line */
   update = 1;

   /* Loop through all settings */
   for (i = 0; owi->data[i].type; i++) {
      /* Allow update for this flag? */
      if (owi->data[i].flags & DATA_FLAG_UPDATE) {
         /* Get value from html request */
	 html = variable_get(owi->data[i].html);
	 /* Get curernt value */
	 current = FILE_CONFIG_GET(owi->file, owi->data[i].directive);

         /* Valid value? */
         if (data_valid(&owi->data[i], html)) {
	    /* Only update non empty values */
	    if (strcmp(html, "")) {
               /* Crypt data? */
	       if (owi->data[i].flags & DATA_FLAG_CRYPT) {
                  html = crypt(html, "OM");
	       }
	    } else {
               html = current;
	    }
	 } else {
	    /* If we get on error we don't update anymore */
            update = 0;
            /* Set error for this variable */
	    variable_error_set(owi->data[i].html, OWI_SYNTAX_INVALID);
	 }

         if (owi->flags & OWI_FLAG_CONFIG) {
	    /* Set this new value only for display */
	    FILE_CONFIG_SET(owi->file, owi->data[i].directive, html, update);
	 } else {
	   /* Update line */
	   file_value_set(owi->file, index_id, variable_get("id"), i, html, update);
	 }
      }
   }
}

/* \fn owi_main(owi)
 * Main entry point for modules
 * \param[in] owi owi module
 */
void owi_main(struct owi_t *owi) {
   /* Command */
   char *command = variable_get("command");

   /* Update? */
   if (!strcmp(command, OWI_BUTTON_UPDATE)) {
      owi_update(owi);
   }

   /* Display header */
   owi_header(owi);

   /* Display list? */
   if (!command || !strcmp(command, "") || !strcmp(command, "list")) {
      if (owi->flags & OWI_FLAG_CONFIG) {
         owi_detail(owi);
         owi_footer(owi, OWI_BUTTON_UPDATE, NULL);
      } else {
         owi_table_header(owi);
	 owi_list(owi);
	 if (owi->flags & OWI_FLAG_HIDE_NEW) {
            /* Display footer */
            owi_footer(owi, NULL);
	 } else {
            /* Display footer */
            owi_footer(owi, OWI_BUTTON_NEW, NULL);
	 }
      }
   /* Show detail screen? */      
   } else if (!strcmp(command, OWI_BUTTON_DETAIL) ||
              !strcmp(command, OWI_BUTTON_UPDATE)) {
      owi_detail(owi);
      /* Display footer */
      owi_footer(owi, OWI_BUTTON_UPDATE, NULL);
   }

   /* File save */
   file_save(owi->file);
}

/* \fn main(argc, argv)
 * Main entry point
 * \param[in] argc number of arguments
 * \param[in] arvv array of arguments
 */
int main(int argc, char **argv) {
   int i;
   char *module = NULL;

   owi_request();

   module = variable_get("module");

   if (!strcmp(module, "")) {
      variable_set("module", "ftp");
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
