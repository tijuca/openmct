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
#include <sys/resource.h>
#include "includes/language.h"
#include "includes/argument.h"
#include "includes/variable.h"
#include "includes/misc.h"
#include "includes/file.h"
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

void owi_header(char *headline) {
   /* Loop counter */
   int i = 0;
   int j = 0;

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
	  "</tr>\n"
	  "<tr>\n"
	  "<td class=\"navigation\">\n"
	  "<table>\n",
	  headline);
   for (i = 0; modules[i].description != NULL; i++) {
      printf("<tr><td>\n");
      for (j = 0; j < modules[i].level * 5; j++) {
         printf("&nbsp;");
      }
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
	  "<td align=\"right\"><a class=\"reload\" href=\"%s?module=%s&amp;command=%s&amp;id=%s\">Reload</a>\n"
	  "</tr>\n"
	  "</table>\n"
	  "<br />\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
	  "<input type=\"hidden\" name=\"command\" value=\"%s\" />\n"
	  "<input type=\"hidden\" name=\"id\" value=\"%s\" />\n",
          variable_get("module"), headline ? headline: "",
	  getenv("SCRIPT_NAME"),
	  variable_get("module"),
	  variable_get("command"),
	  variable_get("id"),
          getenv("SCRIPT_NAME"),
          variable_get("module"),
	  variable_get("command"),
	  variable_get("id"));
}

void owi_footer() {
   printf("</td>\n"
          "</tr>\n"
	  "</table>\n"
          "</form>\n"
          "</td>\n"
          "</tr>\n"
	  "</table>\n"
	  "<br />\n"
	  "<table class=\"footer\">\n"
	  "<tr>\n"
	  "<td>OpenMCT. All rights reserved.</td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</html>\n");
}

void owi_headline(int size, char *headline) {
   printf("<h%d>%s</h%d>\n",
          size,
	  headline,
	  size);
}

void owi_outside_open() {
   printf("<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n"
          "<table class=\"data\" width=\"100%%\">\n");
}

void owi_outside_close(char *value) {
   printf("</table>\n"
          "<table width=\"100%%\">\n"
	  "<tr>\n"
          "<td width=\"100%%\" align=\"right\"><input type=\"button\" class=\"button\" onClick=\"javascript:document.forms[0].command.value='%s';document.forms[0].submit()\" value=\"%s\" /></td>\n"
          "</tr>\n"
	  "</table>\n",
	  value, value);
}

void owi_error(char *value) {
   if (!value) {
      value = variable_get("error");
   }
   if (strcmp(value, "")) {
      printf("<img class=\"error\" src=\"/images/icon_warning.png\" alt=\"%s\" title=\"%s\"/>\n",
             value, value);
   }
}

void owi_box_error() {
   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"boxerror\">%s</div>\n",
             variable_get("error"));
   }
}

void owi_box_info() {
   if (strcmp(variable_get("info"), "")) {
      printf("<div class=\"boxinfo\">%s</div>\n",
             variable_get("info"));
   }
}

void owi_table_header(struct file_t *f) {
   /* Loop */
   int i = 0;

   printf("<tr>\n"
          "<td class=\"list_header\"></td>\n");

   for (i = 0; f->fd[i].html != NULL; i++) {
      if (f->fd[i].flags & FILE_DATA_FLAG_LIST) {
         printf("<td class=\"list_header header_%s_%s\">%s</td>\n",
	        variable_get("module"), f->fd[i].html, f->fd[i].name);
      }
   }
   printf("<td class=\"list_header header_%s_action\">%s</td>\n",
           variable_get("module"), "Action");

   /* Start form / external table / scroll area / internal table*/
   printf("</tr>\n");
}

void owi_data_name(struct file_data_t *ini) {
   printf("%s\n", ini->name);
}

void owi_data_value(struct file_data_t *ini, int flags) {
   /* Loop */
   int i = 0;
   /* Valid values for html elements */
   char **valid_values;

   if (ini->flags & flags) {
   /* Checkbox? */
   if (ini->type == FILE_DATA_TYPE_CHECKBOX) {
      /* Parse valid values from string */
      valid_values = argument_parse(ini->valid, "|");
      /* Print checkbox */
      printf("<input type=\"checkbox\" class=\"checkbox\" name=\"%s\" value=\"%s\" %s /> ",
             ini->html,
	     valid_values[0],
             ini->current && !strcmp(ini->current, valid_values[0]) ?  "checked" : "");
      /* Free valid values */
      free(valid_values);
   /* Selectbox? */
   } else if (ini->type == FILE_DATA_TYPE_SELECT) {
      /* Parse valid values from string */
      valid_values = argument_parse(ini->valid, "|");

      /* Print selectbox */
      printf("<select name=\"%s\">\n", ini->html);
      /* Loop through values */
      for (i = 0; valid_values[i] != NULL; i++) {
         /* Flag for current value */
         int current_value = 0;
	 /* Current value? */
	 if (ini->current && ini->current &&
	    strcmp(ini->current, valid_values[i])) {
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
         
   } else if (ini->type == FILE_DATA_TYPE_TEXT) {
      printf("<input type=\"text\" class=\"%s\" name=\"%s\" value=\"%s\" />",
	     strcmp(variable_error_get(ini->html), "") ? "errortext" : "text",
             ini->html,
	     ini->flags & FILE_DATA_FLAG_DONTFILL ? "" : ini->current ? ini->current : "");
      if (strcmp(variable_error_get(ini->html), "")) {
         printf("&nbsp;");
	 owi_error(variable_error_get(ini->html));
      } 
      printf("<br />\n");
   } else if (ini->type == FILE_DATA_TYPE_PASSWORD) {
      printf("<input type=\"password\" class=\"%s\" name=\"%s\" value=\"%s\" />",
             strcmp(variable_error_get(ini->html), "") ? "errortext" : "text",
             ini->html,
             ini->flags & FILE_DATA_FLAG_DONTFILL ? "" : ini->current ? ini->current : "");
      if (strcmp(variable_error_get(ini->html), "")) {
         printf("&nbsp;");
         owi_error(variable_error_get(ini->html));
      }
      printf("<br />\n");
      
   } else if (ini->type == FILE_DATA_TYPE_TEXTAREA) {
      printf("<textarea name=\"%s\" class=\"%s\" rows=\"4\" cols=\"55\">%s</textarea>",
	     strcmp(variable_error_get(ini->html), "") ? "errortext" : "text",
             ini->html,
	     ini->current ? ini->current : "");
      if (strcmp(variable_error_get(ini->html), "")) {
         printf("<br />\n");
         owi_error(variable_error_get(ini->html));
      }
      printf("<br />\n");
   }
   if (ini->description) {
      printf("%s", ini->description);
   }
   } else {
      printf("%s<br />\n", ini->current ? ini->current : "");
   }
}

void owi_data_detail(struct file_t *f, int flags) {
   /* Loop */
   int i = 0;

   /* Loop through all configs */
   for (i = 0; f->fd[i].html != NULL; i++) {
      if (f->fd[i].type != FILE_DATA_TYPE_INTERNAL) {
         printf("<tr>\n"
                "<td class=\"detail_name detail_name_%s_%s\">\n",
                variable_get("module"), f->fd[i].html);

         /* Display name */
         owi_data_name(&f->fd[i]);

         printf("</td>\n"
                "<td class=\"detail_value detail_value_%s_%s\">\n",
	        variable_get("module"), f->fd[i].html);

         /* Display value */
         owi_data_value(&f->fd[i], flags);

         printf("</td>\n"
                "</tr>\n");
      }	    
   }
}

void owi_data_list(struct file_t *f) {
   /* Loop */
   int j = 0;
   /* id */
   char *id = NULL;

   printf("<tr onmouseover=\"this.className='mover';\" onmouseout=\"this.className='mout';\">\n");

   printf("<td class=\"list_data icon_%s\"></td>\n",
          variable_get("module"));

   for (j = 0; f->fd[j].html != NULL; j++) {
      if (f->fd[j].flags & FILE_DATA_FLAG_LIST) {
         printf("<td class=\"list_data list_%s_%s\">%s</td>\n",
	        variable_get("module"), f->fd[j].html, f->fd[j].current ? f->fd[j].current : "");
      }
      if (f->fd[j].flags & FILE_DATA_FLAG_ID) {
         id = f->fd[j].current;
      }
   }

   printf("<td class=\"list_data list_%s_%s\">\n"
          "<a href=\"#\" class=\"details\" onClick=\"javascript:document.forms[0].command.value='%s';document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n"
          "<a href=\"#\" class=\"delete\" onClick=\"javascript:document.forms[0].command.value='%s';document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n"
	  "</td>\n",
	  variable_get("module"), "action",
	  OWI_BUTTON_DETAIL, id, OWI_BUTTON_DETAIL,
	  OWI_BUTTON_DELETE, id, OWI_BUTTON_DELETE);

   printf("</tr>\n");
}

/* \fn owi_list(f, ...)
 * Show all settings from system
 * \param[in] f file structure
 */
void owi_list(struct file_t *f, ...) {
   /* Dynamic number of arguments */
   va_list list;
   /* Current file structure */
   struct file_t *current;

   /* Print info box if variable info is set */
   owi_box_info();

   /* Print error box if variable info is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open();

   /* Start at first element */
   va_start(list, f);

   /* Loop through all parameter */
   for (current = f;
        current != NULL;
	current = va_arg(list, struct file_t *)) {
      if (current->type == FILE_TYPE_LINE) {
         /* Display table header */
         owi_table_header(current);

         /* Reset line to zero */
         current->line_current = 0;

         /* Reset search */
         current->line_search = 0;

         /* Loop through all entries from file */
         while (file_data_get_next(current)) {
            /* Display */
            owi_data_list(current);
         }
      } else if (current->type == FILE_TYPE_SECTION) {
         /* Display all settings in HTML */
         owi_data_detail(current, FILE_DATA_FLAG_UPDATE);
      }
   }

   /* Print Submit button */
   owi_outside_close(OWI_BUTTON_UPDATE);
}

/* \fn owi_detail(f, id)
 * Show one line with settings from file structure
 * \param[in] f file structure
 * \param[in] id 
 */
void owi_detail_id(struct file_t *f, char *id) {
   /* Reset current line */
   f->line_current = 0;
   /* Reset line search */
   f->line_search = 0;

   /* User found? */
   if (file_data_find(f, id)) {
      /* Display details */
      owi_list(f, NULL);
   }
}

/* \fn owi_update()
 * Update FTP configuration file
 * \param[in] f file structore for ftp settings
 */
void owi_update(struct file_t *f, char *info, char *error) {
   /* Update ini configuration */
   file_data_update(f);

   /* Errors during update? */
   if (!strcmp(variable_get("error"), "")) {
      /* Save result in ftp file */
      file_save(f);

      /* Set info message box */
      variable_set("info", info);
   } else {
      /* Set error message box */
      variable_set("error", error);
   }
}

/* \fn owi_update()
 * Update FTP configuration file
 * \param[in] f file structore for ftp settings
 */
void owi_update_id(struct file_t *f, char *id, char *info, char *error) {
   /* Reset current line */
   f->line_current = 0;
   /* Reset line search */
   f->line_search = 0;

   /* User found? */
   if (file_data_find(f, id)) {
      /* Display details */
      owi_update(f, info, error);
      /* Error found? */
      if (strcmp(variable_get("error"), "")) {
         /* Display detail again */
         owi_list(f, NULL);
      } else {
         /* Display list view */
         owi_list(f, NULL);
      }
   } else {
      /* Display list view */
      owi_list(f);
   }
}

/* \fn owi_delete_id(f, id)
 * Show one line with settings from file structure
 * \param[in] f file structure
 * \param[in] id
 */
void owi_delete_id(struct file_t *f, char *id) {
   /* Reset current line */
   f->line_current = 0;
   /* Reset line search */
   f->line_search = 0;

   /* User found? */
   if (file_data_find(f, id)) {
      /* Delete entry */
      file_action(f, FILE_LINE_DEL, NULL);
      /* Save */
      file_save(f);
   }
   /* Display */
   owi_list(f);
}

/* \fn owi_new(f)
 * Show all settings for add
 * \param[in] f file structure
 */
void owi_new(struct file_t *f) {
   /* Print info box if variable info is set */
   owi_box_info();

   /* Print error box if variable info is set */
   owi_box_error();

   /* Print outside table content */
   owi_outside_open();

   /* Display all settings in HTML */
   owi_data_detail(f, FILE_DATA_FLAG_ADD);

   /* Print Submit button */
   owi_outside_close(OWI_BUTTON_ADD);
}

void owi_add(struct file_t *f) {
}

int main(int argc, char **argv) {
   int i;
   char *module = NULL;

   owi_request();

   module = variable_get("module");

   if (!strcmp(module, "")) {
      variable_set("module", "user");
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
