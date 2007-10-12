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
#include "includes/misc.h"

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
   /* Index counter */
   int i = 0;

   /* Start form / external table / scroll area / internal table*/
   printf("<h3>%s</h3>\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
          "<input type=\"hidden\" name=\"command\" value=\"\" />\n"
	  "<table class=\"outside\">\n"
	  "<tr>\n"
	  "<td>\n"
          "<table class=\"list\">\n"
          "<thead>\n"
          "<tr>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "</tr>\n"
	  "</thead>\n"
          "<tbody>",
	  USER_HEADLINE,
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          USER_TABLE_DESCRIPTION,
          USER_TABLE_GECOS,
          USER_TABLE_ACTION);

   /* Start at first password entry */
   i = 0;
   /* Loop through all user entries in passwd file */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Print entry */
      printf("<tr onmouseover=\"this.className='mover';\""
             " onmouseout=\"this.className='mout';\">\n"
             "<td width=\"200\">%s</td>\n"
             "<td width=\"212\">%s</td>\n"
             "<td width=\"100\">"
             "<input type=\"button\" onClick=\"location='%s?module=%s&amp;command=detail&amp;id=%s'\" value=\"%s\" />&nbsp;"
             "<input type=\"button\" onClick=\"location='%s?module=%s&amp;command=delete&amp;id=%s'\" value=\"%s\" />"
             "</td>\n"
             "</tr>\n",
             argument_get_part(passwd, 0),
             argument_get_part(passwd, 4),
             getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(passwd, 0),
             USER_BUTTON_MODIFY,
             getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(passwd, 0),
             USER_BUTTON_DELETE);
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
	  "<input type=\"button\" onClick=\"location='%s?module=%s&amp;command=new'\" value=\"%s\" />\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
          "</form>\n",
	  getenv("SCRIPT_NAME"),
	  variable_get("module"),
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
   printf("<h3>%s</h3>\n",
          USER_HEADLINE);

   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"error\">%s</div>\n",
             variable_get("error"));
   }

   printf("<table class=\"outside\">\n"
	  "<tr>\n"
	  "<td>\n");

   /* Loop through all user entries in /etc/passwd */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(passwd, 0), username)) {
         printf("<form action=\"%s\" method=\"post\">\n"
                "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\" />\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\" />\n"
                "<table class=\"detail\">\n"
                "<tr>\n"
                "<td width=\"200\" class=\"description\">%s</td>\n"
                "<td width=\"312\" class=\"value\">%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"password\" name=\"password\" /><br />%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"text\" name=\"gecos\" value=\"%s\" /><br />%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"checkbox\" name=\"shell\" value=\"y\" %s /><br />%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td></td>\n"
		"<td>\n"
                "<input type=\"button\" onClick=\"javascript:document.forms[0].submit()\" value=\"%s\" />\n"
                "</td>\n"
	        "</tr>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
		variable_get("module"),
                argument_get_part(passwd, 0),
                USER_TABLE_DESCRIPTION,
                argument_get_part(passwd, 0),
                USER_TABLE_NEW_PASSWORD,
	        USER_PASSWORD_DESCRIPTION,
                USER_TABLE_GECOS,
                argument_get_part(passwd, 4),
	        USER_GECOS_DESCRIPTION,
                USER_TABLE_SHELL,
                strcmp(argument_get_part(passwd, 6), USER_SHELL_FALSE) == 0 ? "" : "checked",              
                USER_SHELL_DESCRIPTION,
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
   /* Error variable */
   char *error = NULL;
   /* password */
   char *password = NULL;
 
   /* Invalid syntax for user description */
   if (!match(variable_get("gecos"), USER_GECOS_VALID)) {
      error = USER_GECOS_INVALID;
   }

   /* Everything fine? */
   if (!error) {
      /* Loop through passwd entries */
      for (i = 0; i < file_line_counter; i++) {
         /* Get passwd entry */
         char **passwd = argument_parse(file_line_get(i), ":");
         /* Passwd entry found? */
         if (!strcasecmp(username, argument_get_part(passwd, 0))) {
	    /* Password set in webinterface? */
	    if (strcmp(variable_get("password"), "")) {
               password = crypt(variable_get("password"), "OM");
	    } else {
               password = argument_get_part(passwd, 1);
	    }
            /* Set new passwd line in memory */
            file_line_action(FILE_LINE_SET, i,
                             "%s:%s:%s:%s:%s:%s:%s",
                             username,
                             password,
                             argument_get_part(passwd, 2),
                             argument_get_part(passwd, 3),
                             variable_get("gecos"),
                             argument_get_part(passwd, 5),
                             strcmp(variable_get("shell"), "y") == 0 ? USER_SHELL_DEFAULT : USER_SHELL_FALSE);
         }
         /* Free passwd entry */
         argument_free(passwd);
      }
   
      /* Save result in user file */
      file_save(USER_FILE);
   
      /* Display user list */
      user_list();
   } else {
      variable_set("error", error);
      user_detail(username);
   }
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
   /* Error variable */
   char *error = NULL;

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
         /* Set error to already exists */
         error = USER_ALREADY_EXISTS;
      } 
      /* Free passwd entry */
      argument_free(passwd);
      /* Found? */
      if (error) {
         break;
      }
   }

   if (!match(username, USER_LOGIN_VALID)) {
      error = USER_LOGIN_INVALID;
   } else if (!match(variable_get("gecos"), USER_GECOS_VALID)) {
      error = USER_GECOS_INVALID;
   }

   /* User not found? */
   if (!error) {
      /* Add new passwd line in memory */
      file_line_action(FILE_LINE_ADD, i,
                       "%s:%s:%d:%d:%s:/home/%s:%s",
                       variable_ltrim(variable_filter(username, ":")),
                       crypt(variable_get("password"), "OM"),
                       start_uid,
                       start_uid,
                       variable_ltrim(variable_filter(variable_get("gecos"), ":")),
		       variable_ltrim(variable_filter(username, ":")),
		       strcmp(variable_get("shell"), "y") == 0 ? USER_SHELL_DEFAULT : USER_SHELL_FALSE);
      /* Save result in user file */
      file_save(USER_FILE);
      /* Display user list */
      user_list();
   } else {
      variable_set("error", error);
      /* Display user add page with error */
      user_new();
   }
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

   /* Display user list */
   user_list();
}

/* \fn user_new()
 * Show the add screen for adding a user 
 */
void user_new() {
   /* Print external table for design */
   printf("<h3>%s</h3>",
          USER_HEADLINE);

   if (strcmp(variable_get("error"), "")) {	  
      printf("<div class=\"error\">%s</div>\n",
             variable_get("error"));
   }	     
	  
   printf("<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
          "<input type=\"hidden\" name=\"command\" value=\"add\" />\n"
	  "<table class=\"outside\">\n"
	  "<tr>\n"
	  "<td>\n"
          "<table class=\"detail\">\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"text\" name=\"id\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"password\" name=\"password\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"text\" name=\"gecos\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"checkbox\" name=\"shell\" value=\"y\" %s /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td></td>\n"
	  "<td>\n"
          "<input type=\"button\" onClick=\"javascript:document.forms[0].submit()\" value=\"%s\" />\n"
          "</td>\n"
	  "</tr>\n"
          "</table>\n"
          "</form>\n",
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          USER_TABLE_DESCRIPTION,
	  variable_get("id"),
	  USER_LOGIN_DESCRIPTION,
          USER_TABLE_NEW_PASSWORD,
	  variable_get("password"),
	  USER_PASSWORD_DESCRIPTION,
          USER_TABLE_GECOS,
	  variable_get("gecos"),
	  USER_GECOS_DESCRIPTION,
	  USER_TABLE_SHELL,
	  strcmp(variable_get("shell"), "y") == 0 ? "checked" : "",
	  USER_SHELL_DESCRIPTION,
          USER_BUTTON_ADD);
}
