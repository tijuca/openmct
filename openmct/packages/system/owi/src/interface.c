/* -*- mode: C; c-file-style: "gnu" -*- */
/* interface.c Interface management
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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/interface.h"
#include "includes/file.h"

/* Define ini configuration tags */
struct file_ini_t interface_ini[] = {
   { INTERFACE_INI_ADDRESS, "address", NULL, NULL, NULL, 0 },
   { INTERFACE_INI_NETMASK, "netmask", NULL, NULL, NULL, 0 },
   { INTERFACE_INI_GATEWAY, "gateway", NULL, NULL, NULL, 0 },
   { INTERFACE_INI_PROVIDER, "provider", NULL, NULL, NULL, 0 },
   { NULL, NULL, NULL, NULL, NULL, 0 }
};

/* \fn interface_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int interface_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header(INTERFACE_HEADLINE);

   /* Read file into memory */
   if (file_open(INTERFACE_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         interface_list();
      }
      /* Free file */
      file_free(INTERFACE_FILE);
   } else {
      /* Print error message */
      owi_headline(1, INTERFACE_HEADLINE);
      owi_headline(2, INTERFACE_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn interface_list()
 * Show all users from system
 */
void interface_list() {
   /* Get command for this module */        
   char *search = variable_get("search");
   /* Index counter */
   int i = 0;
   /* Match counter */
   int j = 0;
   /* Old interface */
   char current_interface[1024] = "";

   /* Start form / external table / scroll area / internal table*/
   printf("<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"command\" value=\"\" />\n"
          "<table class=\"%s\">\n"
          "<tr>\n"
	  "<td>\n"
	  "<h1>%s</h1>\n"
          "<br />%s<br /><br />\n"
          "<table width=\"100%%\">\n"
          "<tr>\n"
	  "<td align=\"right\">"
	  "<input type=\"text\" name=\"search\" value=\"%s\" />&nbsp;"
	  "<input type=\"submit\" value=\"Suchen\" /></td>\n"
	  "</tr>\n"
	  "</table>\n"
          "<div class=\"%s\">\n"
          "<div class=\"%s\">\n"
          "<table class=\"%s\">\n"
          "<thead>\n"
          "<tr>\n"
          "<th width=\"80\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "</tr>\n"
	  "</thead>\n"
          "<tbody>",
          getenv("SCRIPT_NAME"),
	  CONTENT_TABLE_CLASS,
	  INTERFACE_HEADLINE,
	  INTERFACE_DESCRIPTION,
	  variable_get("search"),
	  CONTENT_DATAGRID_HEADER,
	  CONTENT_DATAGRID_CONTENT,
	  CONTENT_TABLE_LIST_CLASS,
          INTERFACE_TABLE_DESCRIPTION,
          INTERFACE_TABLE_IPADDRESS,
          INTERFACE_TABLE_NETMASK,
          INTERFACE_TABLE_BROADCAST,
          INTERFACE_TABLE_ACTION);

   /* Start at first password entry */
   i = 0;
   /* Loop through all user entries in passwd file */
   while (i <= file_line_counter) {
      /* Parse interface entry */
      char **interface = NULL;
      /* Pseudo line not reached? */
      if (i < file_line_counter) {
         /* Get and parse line */
         interface = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      }
      /* Auto line? (= interface start) */
      if ( (interface && interface[0] && !strcasecmp(interface[0], "auto")) ||
           i == file_line_counter) {
         if (strcmp(current_interface, "")) {
            /* Print network interface */
            printf("<tr onmouseover=\"this.className='%s';\""
                " onmouseout=\"this.className='%s';\">\n"
                "<td width=\"80\">%s</td>\n"
                "<td width=\"160\">%s</td>\n"
                "<td width=\"160\">%s</td>\n"
                "<td width=\"160\">%s</td>\n"
                "<td width=\"160\">"
		"<input type=\"button\" onClick=\"location='%s?command=detail&amp;id=%s'\" value=\"%s\" />&nbsp;"
		"<input type=\"button\" onClick=\"location='%s?command=delete&amp;id=%s'\" value=\"%s\" />"
		"</td>\n"
                "</tr>\n",
                CONTENT_TABLE_CLASS_MOUSEOVER,
                CONTENT_TABLE_CLASS_MOUSEOUT,
                current_interface,
                interface_ini[0].current ? interface_ini[0].current[1] : "",
                interface_ini[1].current ? interface_ini[1].current[1] : "",
                interface_ini[2].current ? interface_ini[2].current[1] : "",
                getenv("SCRIPT_NAME"),
                current_interface,
		INTERFACE_BUTTON_MODIFY,
		getenv("SCRIPT_NAME"),
                current_interface,
		INTERFACE_BUTTON_DELETE);
         }
	 /* Clear current interface */
	 memset(current_interface, sizeof(current_interface), 0);
	 if (interface) {
	    /* Copy current_interface from first argument from "auto" line */
	    strncpy(current_interface, interface[1], sizeof(current_interface));
	 }
	 /* Loop through all ini settings */
         for (j = 0; interface_ini[j].variable != NULL; j++) {
	    /* Argument set for this line */
            if (interface_ini[j].current) {
	       /* Free argument */
	       argument_free(interface_ini[j].current);  
	       /* And set argument pointer to zero */
	       interface_ini[j].current = NULL;
	    }
         }
	 /* Not last section in file? */
	 if (interface) {
	    /* Free interface entry */
	    argument_free(interface);
	 }
      } else {
         /* For lookup */
         int interface_ini_found = 0;
         /* Parse through all available commands */
         for (j = 0; interface_ini[j].variable != NULL; j++) {
	    /* Argument command matches? */
            if (interface && interface[0] &&
	        !strcasecmp(interface[0], interface_ini[j].variable)) {
               /* Set pointer */
               interface_ini[j].current = interface;
	       /* Set found to true */
	       interface_ini_found = 1;
            }
         }
	 /* Free interface information? */
	 if (!interface_ini_found && interface) {
            /* Free argument */
	    argument_free(interface);
	 }
      }
      /* Increase counter */
      i++;
   }
   /* Print table footer / close internal table / close scroll / print footer / ... */
   printf("</tbody>\n"
          "</table>\n"
          "</div>\n"
	  "</div>\n"
          "<table width=\"100%%\">\n"
	  "<tr>\n"
	  "<td colspan=\"7\" align=\"right\">\n"
	  "<input type=\"button\" onClick=\"location='%s?command=new'\" value=\"%s\" />"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</td>\n"
          "</tr>\n"
	  "</table>\n"
          "</form>\n",
	  getenv("SCRIPT_NAME"),
	  INTERFACE_BUTTON_NEW);
}
