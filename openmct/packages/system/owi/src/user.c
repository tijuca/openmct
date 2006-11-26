/* -*- mode: C; c-file-style: "gnu" -*- */
/* user.c User management
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
#include "includes/user.h"
#include "includes/file.h"

/* \fn user_main(argc, argv)
 * Show all users from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int user_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header(USER_HEADLINE);

   /* Read file into memory */
   if (file_open(USER_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         user_list();
      /* show user detail */
      } else if (!strcasecmp(command, "detail")) {
         /* Show user detail */
         user_detail(variable_get("id"));
      } else if (!strcasecmp(command, "update")) {
         user_update(variable_get("id"));
      } else if (!strcasecmp(command, "new")) {
         user_new(variable_get("id"));
      } else if (!strcasecmp(command, "add")) {
         user_add(variable_get("id"));
      } else if (!strcasecmp(command, "delete")) {
         user_delete(variable_get("id"));
      }
      /* Free file */
      file_free(USER_FILE);
   } else {
      /* Print error message */
      owi_headline(1, USER_HEADLINE);
      owi_headline(2, USER_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn user_list()
 * Show all users from system
 */
void user_list() {
   /* Get command for this module */        
   char *search = variable_get("search");
   /* Index counter */
   int i = 0;

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
          "<div class=\"searchbar\">\n"
          "<input class=\"searchbox\" type=\"text\" name=\"search\" value=\"%s\" /><a href=\"#\" onclick=\"javascript:document.forms[0].reset()\" class=\"searchreset\" title=\"L&ouml;schen\"><img src=\"images/reset.gif\" id=\"searchreset\" border=\"0\" alt=\"\" /></a>\n"
          "</div>\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
          "<table class=\"%s\" cellpadding=\"0\" cellspacing=\"0\">\n"
          "<thead>\n"
          "<tr>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "</tr>\n"
	  "</thead>\n"
          "<tbody>",
          getenv("SCRIPT_NAME"),
	  CONTENT_TABLE_CLASS,
	  USER_HEADLINE,
	  USER_DESCRIPTION,
	  variable_get("search"),
	  CONTENT_TABLE_LIST_CLASS,
          USER_TABLE_DESCRIPTION,
          USER_TABLE_GECOS,
          USER_TABLE_DIRECTORY,
          USER_TABLE_SHELL,
          USER_TABLE_ACTION);

   /* Start at first password entry */
   i = 0;
   /* Loop through all user entries in passwd file */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Search string specified? */
      if (!search || !strcmp(search, "") ||
          (search && 
           (strstr(argument_get_part(passwd, 0), search) ||
            strstr(argument_get_part(passwd, 3), search) ||
            strstr(argument_get_part(passwd, 4), search) ||
            strstr(argument_get_part(passwd, 5), search)))) {
         /* Print entry */
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
                argument_get_part(passwd, 0),
                argument_get_part(passwd, 4),
                argument_get_part(passwd, 5),
                argument_get_part(passwd, 6),
                getenv("SCRIPT_NAME"),
                argument_get_part(passwd, 0),
		USER_BUTTON_MODIFY,
		getenv("SCRIPT_NAME"),
		argument_get_part(passwd, 0),
		USER_BUTTON_DELETE);
      }
      /* Increase counter */
      i++;
      /* Free passwd entry */
      argument_free(passwd);
   }
   /* Print table footer / close internal table / close scroll / print footer / ... */
   printf("</tbody>\n"
          "</table>\n"
          "<table width=\"100%%\">\n"
	  "<tr>\n"
	  "<td colspan=\"7\" align=\"right\">\n"
	  "<a class=\"%s\" href=\"#\" onClick=\"location='%s?command=new'\"><div class=\"%s\">%s</div></a>\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</td>\n"
          "</tr>\n"
	  "</table>\n"
          "</form>\n",
	  CONTENT_LINK_AQUA_CLASS,
	  getenv("SCRIPT_NAME"),
	  CONTENT_BUTTON_AQUA_CLASS,
	  USER_BUTTON_NEW);
}

/* \fn user_detail(username)
 * Show one user account
 * \param[in] username user that will be displayed
 */
void user_detail(char *username) {
   /* User found? */
   int user_found = 0;
   /* Index counter */
   int i = 0;

   /* Print external table for design */
   printf("<table class=\"%s\">\n"
          "<tr>\n"
          "<td>\n"
	  "<h1>%s</h1>\n"
	  "<br />%s<br /><br />\n",
          CONTENT_TABLE_CLASS,
	  USER_HEADLINE,
	  USER_DETAIL);

   /* Loop through all user entries in /etc/passwd */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(passwd, 0), username)) {
         printf("<form action=\"%s\" method=\"post\">\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\" />\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\" />\n"
                "<table class=\"%s\" width=\"100%%\">\n"
                "<tr>\n"
                "<td width=\"250\">%s</td>\n"
                "<td>%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"password\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"gecos\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"directory\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"shell\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "</table>\n"
                "<table width=\"100%%\">\n"
                "<tr>\n"
                "<td colspan=\"2\" align=\"right\">\n"
                "<a href=\"#\" onClick=\"javascript:document.forms[0].submit()\" class=\"%s\"><div class=\"%s\">%s</div></a>\n"
                "</td>\n"
	        "</tr>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
                argument_get_part(passwd, 0),
		CONTENT_TABLE_BOX_CLASS,
                USER_TABLE_DESCRIPTION,
                argument_get_part(passwd, 0),
                USER_TABLE_NEW_PASSWORD,
                USER_TABLE_UID,
                argument_get_part(passwd, 2),
                USER_TABLE_GID,
                argument_get_part(passwd, 3),
                USER_TABLE_GECOS,
                argument_get_part(passwd, 4),
                USER_TABLE_DIRECTORY,
                argument_get_part(passwd, 5),
                USER_TABLE_SHELL,
                argument_get_part(passwd, 6),
		CONTENT_LINK_AQUA_CLASS,
		CONTENT_BUTTON_AQUA_CLASS,
		USER_BUTTON_UPDATE);


         /* Set user found to one */
         user_found = 1;
      }
      /* Increase index counter */
      i++;
      /* Free passwd entry */
      argument_free(passwd);
   }

   /* No user found? */
   if (!user_found) {
      /* Print information screen */
      owi_headline(2, USER_NOT_FOUND);
   }

   /* Close external table */
   printf("</td>\n"
          "</tr>\n"
	  "</table>\n");
}

/* \fn user_update(username)
 * Update an entry in password file
 * \param[in] username username that will be updated
 */
void user_update(char *username) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(username, argument_get_part(passwd, 0))) {
         /* Set new passwd line in memory */
         file_line_action(FILE_LINE_SET, i,
                          "%s:%s:%s:%s:%s:%s:%s",
                          username,
                          crypt(variable_get("password"), "OM"),
                          argument_get_part(passwd, 2),
                          argument_get_part(passwd, 3),
                          variable_ltrim(variable_filter(variable_get("gecos"), ":")),
                          variable_ltrim(variable_filter(variable_get("directory"), ":")),
                          variable_ltrim(variable_filter(variable_get("shell"), ":")));
      }
      /* Free passwd entry */
      argument_free(passwd);
   }
   
   /* Save result in user file */
   file_save(USER_FILE);

   /* Display user */
   user_detail(username);
}

/* \fn user_add(username)
 * Execute a new user now
 * \param[in] username username that will be updated
 */
void user_add(char *username) {
   /* Index counter */
   int i;
   /* Max uid */
   unsigned int start_uid = 1000;
   /* User already found? */
   int found = 0;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Current uid start uid found? */
      if (atoi(argument_get_part(passwd, 2)) == start_uid) {
         /* Increase start uid */
         start_uid++;
	 /* And scan again */
	 i = 0;
      }
      /* New user already in user database */
      if (!strcasecmp(argument_get_part(passwd, 0), 
          variable_ltrim(variable_filter(username, ":")))) {
         /* Set found to true */
         found = 1;
      } 
      /* Free passwd entry */
      argument_free(passwd);
      /* Found? */
      if (found) {
         break;
      }
   }
   /* User not found? */
   if (!found) {
      /* Add new passwd line in memory */
      file_line_action(FILE_LINE_ADD, i,
                       "%s:%s:%d:%d:%s:%s:%s",
                       variable_ltrim(variable_filter(username, ":")),
                       crypt(variable_get("password"), "OM"),
                       start_uid,
                       start_uid,
                       variable_ltrim(variable_filter(variable_get("gecos"), ":")),
                       variable_ltrim(variable_filter(variable_get("directory"), ":")),
                       variable_ltrim(variable_filter(variable_get("shell"), ":")));
      /* Save result in user file */
      file_save(USER_FILE);
   }

   /* Display new user*/
   user_detail(username);
}

/* \fn user_delete(username)
 * Delete an entry from password file
 * \param[in] username username that will be deleted
 */
void user_delete(char *username) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(username, argument_get_part(passwd, 0))) {
         /* Set new passwd line in memory */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free passwd entry */
      argument_free(passwd);
   }

   /* Save result in user file */
   file_save(USER_FILE);

   /* Display user */
   user_list();
}

/* \fn user_new()
 * Show the add screen for adding a user 
 */
void user_new() {
   /* Print external table for design */
   printf("<table class=\"%s\">\n"
          "<tr>\n"
          "<td>\n"
	  "<h1>%s</h1>"
	  "<br />%s<br /><br />\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"command\" value=\"add\" />\n"
          "<table class=\"%s\" width=\"100%%\">\n"
          "<tr>\n"
          "<td width=\"250\">%s</td>\n"
          "<td><input type=\"text\" name=\"id\" /></td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td>%s</td>\n"
          "<td><input type=\"password\" /></td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td>%s</td>\n"
          "<td><input type=\"text\" name=\"gecos\" /></td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td>%s</td>\n"
          "<td><input type=\"text\" name=\"directory\" /></td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td>%s</td>\n"
          "<td><input type=\"text\" name=\"shell\" /></td>\n"
          "</tr>\n"
          "</table>\n"
          "<table width=\"100%%\">\n"
          "<tr>\n"
          "<td colspan=\"2\" align=\"right\">\n"
          "<a href=\"#\" onClick=\"javascript:document.forms[0].submit()\" class=\"%s\"><div class=\"%s\">%s</div></a>\n"
          "</td>\n"
	  "</tr>\n"
          "</table>\n"
          "</form>\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n",
          CONTENT_TABLE_CLASS,
	  USER_HEADLINE,
	  USER_NEW,
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_BOX_CLASS,
          USER_TABLE_DESCRIPTION,
          USER_TABLE_NEW_PASSWORD,
          USER_TABLE_GECOS,
          USER_TABLE_DIRECTORY,
          USER_TABLE_SHELL,
          CONTENT_LINK_AQUA_CLASS,
          CONTENT_BUTTON_AQUA_CLASS,
          USER_BUTTON_ADD);
}
