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
#include "includes/string.h"
#include "includes/array.h"
#include "includes/data.h"
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
#include "includes/shell.h"
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
	  string_value(owi->headline));
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
          owi_get_value("module"), owi->headline ? string_value(owi->headline) : "",
	  getenv("SCRIPT_NAME"),
	  owi_get_value("module"),
	  owi_get_value("id"),
          getenv("SCRIPT_NAME"),
          owi_get_value("module"),
	  owi_get_value("command"),
	  owi_get_value("id"));

   if (string_compare_value(owi_get("error"), "")) {
      printf("<div class=\"boxerror\">%s</div>\n",
             owi_get_value("error"));
   } else if (string_compare_value(owi_get("info"), "")) {
      printf("<div class=\"boxinfo\">%s</div>\n",
             owi_get_value("info"));
   }

   printf("<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n"
          "<table class=\"data\" width=\"100%%\">\n");
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
                 owi_get_value("module"), owi->data[i].html, owi->data[i].name);
      }
   }
   /* Display action column? */
   if (owi->flags & OWI_FLAG_ACTION) {
      printf("<td class=\"list_header header_%s_action\">%s</td>\n",
             owi_get_value("module"), "Action");
   }	     
   /* Start form / external table / scroll area / internal table*/
   printf("</tr>\n");
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
   for (current = button; current != NULL; current = va_arg(list, char*)) {
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
	  "<td>OpenMCT is &copy; 2008 by OpenMCT. All rights reserved.<br />"
	  "<a href=\"http://www.openmct.org\" target=\"_blank\">http://www.openmct.org</a></td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</html>\n");
}

/* \fn owi_display(owi, index, current, flags)
 * Display one value in html (depending on type, flags)
 * \param[in] owi handler
 * \param[in] index data index
 * \param[in] current current data value
 * \param[in] flags flags for display behavior
 */
void owi_display(struct owi_t *owi, unsigned int index, struct string_t *current, unsigned int flags) {
   /* Loop */
   int i = 0;
   /* Valid values for html select/checkbox elements */
   struct array_t *valid = NULL;
   /* Current data */
   struct data_t *data = &(owi->data[index]);
   /* Error variable */
   struct string_t *error = NULL;
   /* Class name for variables */
   struct string_t *class = NULL;

   /* Flags set so its not just display values? */
   if (data->flags & flags) {
      /* Create error variable */
      error = string_insert_value(string_copy_value(data->html),
                                  0,
                                  OWI_ERROR_PREFIX);
      class = string_copy_value("text");
      /* Checkbox? */
      if (data->type == DATA_TYPE_CHECKBOX) {
         /* Parse valid values from string */
         valid = string_split_value(string_copy_value(data->valid), "|");
         /* Print checkbox */
         printf("<input type=\"checkbox\" class=\"checkbox\" "
	        "name=\"%s\" value=\"%s\" %s /> ",
                data->html,
                string_value(array_value(valid, 0)),
                current && !string_compare(current, array_value(valid, 0)) ?  "checked" : "");
         /* Free valid values */
         array_free(&valid);
      /* Selectbox? */
      } else if (data->type == DATA_TYPE_SELECT) {
         /* Parse valid values from string */
         valid = string_split_value(string_copy_value(data->valid), "|");
   
         /* Print selectbox */
         printf("<select name=\"%s\" class=\"%s\">\n",
	        data->html,
                string_value(class));
         /* Loop through values */
         for (i = 0; i < array_length(valid); i++) {
            /* Flag for current value */
            int current_value = 0;
            /* Current value? */
            if (current && string_compare(current, array_value(valid, i))) {
               current_value = 1;
            }

            /* Print on stdout */
            printf("<option value=\"%s\" %s>%s</option>\n",
                   string_value(array_value(valid, i)),
                   current_value == 0 ? " selected" : "",
                   string_value(array_value(valid, i)));
         }
         printf("</select><br />\n");
         /* Free valid values */
         array_free(&valid);
      } else if (data->type == DATA_TYPE_TEXT) {
         printf("<input type=\"text\" class=\"%s\" name=\"%s\" value=\"%s\" />",
   	        string_value(class),
                data->html,
   	        data->flags & DATA_FLAG_DONT_FILL ? "" : current ? string_value(current) : "");
         printf("<br />\n");
      } else if (data->type == DATA_TYPE_PASSWORD) {
         printf("<input type=\"password\" class=\"%s\" name=\"%s\" "
	        "value=\"%s\" />",
                string_value(class),
                data->html,
                data->flags & DATA_FLAG_DONT_FILL ? "" : current ? string_value(current) : "");
         printf("<br />\n");
      } else if (data->type == DATA_TYPE_TEXTAREA) {
         printf("<textarea name=\"%s\" class=\"%s\" rows=\"4\" cols=\"55\">%s"
	        "</textarea>",
                data->html,
                string_value(class),
                current ? string_value(current) : "");
         printf("<br />\n");
      }
      if (data->description) {
         printf("%s", data->description);
      }
   } else {
      printf("%s<br />\n", current ? string_value(current) : "");
   }
}

/* \fn owi_list(owi)
 * Print list screen for current module
 * \param[in] owi owi settings
 */
void owi_list(struct owi_t *owi) {
   /* Loop */
   unsigned int j, i = 0;
   /* Current line */
   struct string_t *line = NULL;
   /* Current columns for this line */
   struct array_t *column = NULL;
   /* Index id for current line */
   struct string_t *id = NULL;

   /* Loop through all lines */
   for (file_iterator_start(owi->file, &i) ;
        i < file_length(owi->file);
	file_iterator_next(owi->file, &i)) {
      /* Set current line */
      line = file_line(owi->file, i);
      /* Parse line */
      column = string_split(line, owi->file->separator);
 
      printf("<tr>\n");

      for (j = 0; owi->data[j].type; j++) {
         if (owi->data[j].flags & DATA_FLAG_LIST) {
            printf("<td class=\"list_data list_%s_%s\">%s</td>\n",
	           owi_get_value("module"),
		   owi->data[j].html,
		   j < array_length(column) ? string_value(array_value(column, j)) : "");
         }
         if (owi->data[j].flags & DATA_FLAG_ID) {
            id = array_value(column, j);
         }
      }

      /* Print action buttons? */
      if (owi->flags & OWI_FLAG_ACTION) {
         printf("<td class=\"list_data list_%s_%s\">\n",
	        owi_get_value("module"), "action");
         /* Show delete button? */
         if (owi->flags & OWI_FLAG_ACTION_DELETE) {
	    printf("<a href=\"#\" class=\"delete\" "
                   "onClick=\"javascript:document.forms[0].command.value='%s';"
                   "document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n",
                   OWI_BUTTON_DELETE, string_value(id), OWI_BUTTON_DELETE);
         }		   
         /* Show detail button? */
         if (owi->flags & OWI_FLAG_ACTION_DETAIL) {
	    printf("<a href=\"#\" class=\"details\" "
                   "onClick=\"javascript:document.forms[0].command.value='%s';"
                   "document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n",
                   OWI_BUTTON_DETAIL, string_value(id), OWI_BUTTON_DETAIL);
         }
	 /* Show kill button? */
         if (owi->flags & OWI_FLAG_ACTION_KILL) {
	    printf("<a href=\"#\" class=\"kill\" "
                   "onClick=\"javascript:document.forms[0].command.value='%s';"
                   "document.forms[0].id.value='%s';document.forms[0].submit();\">%s</a>\n",
                   OWI_BUTTON_KILL, string_value(id), OWI_BUTTON_KILL);
         }
      }		
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
   struct string_t *current = NULL;
   /* Search value */
   struct string_t *search = NULL;

   /* Loop through all settings */
   for (i = 0; owi->data[i].type; i++) {
      /* Display value? */
      if (owi->data[i].type != DATA_TYPE_INTERNAL) {
         printf("<tr>"
                "<td class=\"detail_name\">%s</td>"
                "<td class=\"detail_value detail_value\">",
                owi->data[i].name);
         /* Search in config file? */
         if (owi->flags & OWI_FLAG_CONFIG) {
	    search = string_copy_value(owi->data[i].directive);
	 } else {
            search = string_copy(owi_get("id"));
	 }
         /* Read current value */ 		
	 current = file_value_get(owi->file,
	                          owi->index_id,
				  search,
                                  owi->flags & OWI_FLAG_CONFIG ? 1 : i);
         /* Display variable */
         owi_display(owi, i, current, DATA_FLAG_UPDATE);
         printf("</tr>\n");
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
   struct string_t *html = NULL;
   /* Current value */
   struct string_t *current = NULL;

   /* Loop through all settings */
   for (i = 0; owi->data[i].type; i++) {
      /* Allow update for this flag? */
      if (owi->data[i].flags & DATA_FLAG_UPDATE) {
         /* Get value from html request */
	 html = owi_get(owi->data[i].html);
	 /* Get current value */
	 current = file_value_get(owi->file,
	                          owi->index_id,
                                  owi->flags & OWI_FLAG_CONFIG ?
                                     string_copy_value(owi->data[i].directive) : owi_get("id"),
                                  owi->flags & OWI_FLAG_CONFIG ?
				     1 : i);
         /* Empty value? Do special handling for empty values */
	 if (!string_compare_value(html, "")) {
	    /* Skip empty values? */
            if (owi->data[i].flags & DATA_FLAG_SKIP_EMPTY) {
               /* Delete this line */
	       file_value_del(owi->file,
	                      owi->index_id,
			      string_copy_value(owi->data[i].directive));
	    /* Use empty value? */
	    } else if (!(owi->data[i].flags & DATA_FLAG_DONT_FILL)) {
	       /* Checkbox? */
	       if (owi->data[i].type == DATA_TYPE_CHECKBOX) {
	          /* Use standard value */
                  html = string_copy_value(owi->data[i].standard);
	       }
	       /* Write empty value to config file */
	       file_value_set(owi->file,
	                      owi->index_id,
                              owi->flags & OWI_FLAG_CONFIG ?
                                 string_copy_value(owi->data[i].directive) : owi_get("id"),
			      owi->flags & OWI_FLAG_CONFIG ?
			         1 : i,
			      html);
	    }
	 /* Nonempty value? */
	 } else {
            /* Syntax defined for current variable? */
	    if (owi->data[i].valid && !data_valid(&owi->data[i], html)) {
               /* Set error for this variable */
	       owi_error_set(owi,
	                     string_copy_value(owi->data[i].html),
	                     string_copy_value(OWI_SYNTAX_INVALID));
	    }
            /* Crypt data? */
	    if (owi->data[i].flags & DATA_FLAG_CRYPT) {
               html = string_copy_value(crypt(string_value(html), "OM"));
	    }
	    /* Already found in file? */
	    if (current) {
	       /* Update line */
	       file_value_set(owi->file,
	                      owi->index_id,
                              owi->flags & OWI_FLAG_CONFIG ?
                                 string_copy_value(owi->data[i].directive) : owi_get("id"),
                              owi->flags & OWI_FLAG_CONFIG ?
			         1 : i,
                              html);
            /* Not found in config file? */
            } else {
	    }
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
   struct string_t *command = owi_get("command");
   /* Index loop */
   unsigned int index;

   /* Open required files if name is set */
   if (owi->file->name) {
      file_open(owi->file);
   }
   if (owi->file_init->name) {
      file_open(owi->file_init);
   }

   /* Get index id for current data - set per default to first column */
   owi->index_id = 0;
   /* Correct flags set and data structure set? */
   if (owi->flags & OWI_FLAG_ROW && owi->data) {
      /* Loop through all settings */
      for (index = 0; owi->data[index].type; index++) {
         /* Index column found? */
         if (owi->data[index].flags & DATA_FLAG_ID) {
            owi->index_id = index;
         }
      }
   }

   /* Update? */
   if (owi->flags & OWI_FLAG_ACTION_UPDATE &&
       !string_compare_value(command, OWI_BUTTON_UPDATE)) {
       owi_update(owi);
   /* Delete */
   } else if (owi->flags & OWI_FLAG_ACTION_DELETE &&
              !string_compare_value(command, OWI_BUTTON_DELETE) &&
              string_compare_value(owi_get("id"), "")) {
      /* Remove by index */
      file_value_del(owi->file,
                     owi->index_id,
		     owi_get("id"));
   }

   /* Display header */
   owi_header(owi);

//   /* Var dump */
//   printf("<code><pre>\n");
//   for (index = 0; index < array_length(owi->request); index++) {
//      printf("[%20s][%20s]<br />\n",
//             string_value(array_key(owi->request, index)),
//             string_value(array_value(owi->request, index)));
//                                   
//   }
//   printf("</pre></code>\n");

   /* Display list? */
   if (!command || !string_compare_value(command, "") ||
       !string_compare_value(command, OWI_BUTTON_LIST) ||
       !string_compare_value(command, OWI_BUTTON_KILL) ||
       !string_compare_value(command, OWI_BUTTON_DELETE)) {
      if (owi->flags & OWI_FLAG_CONFIG) {
         owi_detail(owi);
	 if (owi->flags & OWI_FLAG_ACTION_UPDATE) {
            owi_footer(owi, OWI_BUTTON_UPDATE, NULL);
	 } else {
            owi_footer(owi, NULL);
	 }
      } else if (owi->flags & OWI_FLAG_ROW) {
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
   } else if (owi->flags & OWI_FLAG_ACTION_UPDATE &&
              (!string_compare_value(command, OWI_BUTTON_DETAIL) ||
              !string_compare_value(command, OWI_BUTTON_UPDATE))) {
      owi_detail(owi);
      /* Display footer */
      if (owi->flags & OWI_FLAG_ACTION_UPDATE) {
         owi_footer(owi, OWI_BUTTON_UPDATE, NULL);
      } else {
         owi_footer(owi, NULL);
      }
   }
}

/* \fn owi_free(owi)
 * Free owi data structure
 */
void owi_free(struct owi_t *owi) {
   if (owi->file->name) {
      file_free(owi->file);
   }
   free(owi->file);
   owi->file = NULL;

   if (owi->file_init->name) {
      file_free(owi->file_init);
   }
   free(owi->file_init);
   owi->file_init = NULL;
}

/* \fn main(argc, argv)
 * Main entry point
 * \param[in] argc number of arguments
 * \param[in] arvv array of arguments
 */
int main(int argc, char **argv) {
   /* owi handler */
   struct owi_t *owi = NULL;
   /* Return value from module */
   int retval = 0;
   /* Index */
   unsigned int index;
   /* Module */
   struct string_t *module = NULL;

   /* Allocate memory for owi object */
   owi = (struct owi_t*)malloc(sizeof(struct owi_t));
   /* Enough memory? */
   if (owi) {
      /* Allocate memory for file objects */
      owi->file = (struct file_t*)malloc(sizeof(struct file_t));
      owi->file_init = (struct file_t*)malloc(sizeof(struct file_t));

      /* Everything ok? */
      if (owi->file && owi->file_init) {
         memset(owi->file, 0, sizeof(struct file_t));
	 memset(owi->file_init, 0, sizeof(struct file_t));
         /* Get request data */
         owi_request(owi);
         /* Get module parameter */
         module = owi_get("module");

         if (!module || !string_compare_value(module, "")) {
            module = string_copy_value(MODULE_DEFAULT);
         }

         /* Lookup correct module */
         for (index = 0; modules[index].name != NULL; index++) {
            if (!string_compare_value(module, modules[index].name)) {
               retval = modules[index].main(owi);
            }
         }
      }

      if (owi->file->name) {
         file_save(owi->file);
      }

      if (owi->file_init->name) {
         file_save(owi->file_init);
      }

      /* Free memory */
      owi_free(owi);
   }

   return retval;
}

/* \fn owi_request(owi)
 * Parses request data from HTTP client (GET or POST request)
 * \param[in] owi handler
 */
void owi_request(struct owi_t *owi) {
   /* Get request method */
   struct string_t *request_method = string_copy_value(getenv("REQUEST_METHOD"));
   /* Get content length */
   struct string_t *content_length = string_copy_value(getenv("CONTENT_LENGTH"));
   /* Query string */
   struct string_t *query_string = string_copy_value(getenv("QUERY_STRING"));
   /* Post data */
   struct string_t *post = NULL;
   /* Content lenght */
   unsigned int length;
   /* Parameter */
   struct array_t *parameter = NULL;
   /* Key value pair */
   struct array_t *keyvalue = NULL;
   /* Index */
   unsigned int index;
   /* Key */
   struct string_t *key = NULL;
   /* Value */
   struct string_t *value = NULL;

   if (!request_method) {
      printf("Not running via CGI\n");
      exit(0);
   }

   /* GET request? */
   if (!string_compare_value(request_method, "GET")) {
      /* Parse parameters */
      parameter = string_split_value(query_string, OWI_SEPARATOR_PARAMETER);
   /* POST request and content_length set? */
   } else if (!string_compare_value(request_method, "POST") && 
              string_compare_value(content_length, "")) {
      /* convert string to integer */
      length = atoi(string_value(content_length));
      /* Allocate memory for post data */
      post = string_init(length);
      /* Read post data */
      if (read(0, string_value(post), length)) {
         /* Parse parameters */
         parameter = string_split_value(post, OWI_SEPARATOR_PARAMETER);
      }
      /* Free post data */
      string_free(&post);
    }

    /* Init request variable */
    owi->request = NULL;
    /* Set key */
    key = string_copy_value("OWI_version");
    /* Set value */
    value = string_copy_value("1.0");
    /* Add to array */
    array_key_add(&(owi->request), key, value);
    /* Free key */
    string_free(&key);
    /* Free value */
    string_free(&value);

    /* Parsing done? */
    if (parameter) {
       /* Loop trough all parameters */
       for (index = 0; index < array_length(parameter); index++) {
          /* Parse key/value pair */
	  keyvalue = string_split_value(array_value(parameter, index),
	                                OWI_SEPARATOR_KEYVALUE);
          /* Decode character encoding (%[0-9A-F][0-9A-F]) */
          key = string_decode(array_value(keyvalue, 0));
	  /* Value set? */
	  if (array_length(keyvalue) > 1) {
	     /* Encode value */
             value = string_decode(array_value(keyvalue, 1));
	  } else {
	     /* Just initialize an empty string */
             value = string_init(0);
	  }
          /* Add to array */
          array_key_add(&(owi->request), key, value);
	  /* Free key */
	  string_free(&key);
	  /* Free value */
	  string_free(&value);
          /* Free temporary key/value array */
	  array_free(&keyvalue);
       }
    }

    /* Free environment variables */
    string_free(&query_string);
    string_free(&content_length);
    string_free(&request_method);
    /* Free temporary parameter read via GET/POST */
    array_free(&parameter);
}

void owi_error_set(struct owi_t *owi, struct string_t *key, struct string_t *value) {
   /* Create errory key variable */
   struct string_t *error_key = string_insert_value(key,
                                                    0,
						    OWI_ERROR_PREFIX);

   /* Add to request array */
   array_key_add(&(owi->request), key, value);

   /* Free error key */
   string_free(&error_key);
}
