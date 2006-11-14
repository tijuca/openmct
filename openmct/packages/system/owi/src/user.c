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
   owi_header("User Management");

   /* Read file into memory */
   if (file_read(USER_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         user_list();
      /* show user detail */
      } else if (!strcasecmp(command, "detail")) {
         /* Show user detail */
         user_detail(variable_get("id"));
      } else if (!strcasecmp(command, "update")) {
         user_update(variable_get("id"), variable_get("password"), variable_get("gecos"),
                     variable_get("directory"), variable_get("shell"));
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

   owi_image("images/user.png", USER_HEADLINE, USER_HEADLINE);
   owi_headline(1, USER_HEADLINE);
   owi_headline(2, USER_DESCRIPTION);

   /* print table headline */
   printf("<div class=\"dataGridHeader\">\n"
          "<div class=\"dataGridContent\">\n"
          "<table class=\"%s\">\n"
          "<thead>\n"
          "<tr>\n"
          "<td width=\"30\">&nbsp;</td>\n"
          "<td width=\"60\">%s</td>\n"
          "<td width=\"90\">%s</td>\n"
          "<td width=\"120\">%s</td>\n"
          "<td width=\"150\">%s</td>\n"
          "<td width=\"120\">%s</td>\n"
          "<td width=\"80\">%s</td>\n"
          "<td></td>\n"
          "</tr>\n"
          "<tbody>",
          CONTENT_TABLE_CLASS,
          USER_TABLE_DESCRIPTION,
          USER_TABLE_UID,
          USER_TABLE_GID,
          USER_TABLE_GECOS,
          USER_TABLE_DIRECTORY,
          USER_TABLE_SHELL);

   /* Loop through all user entries in passwd file */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Search string specified? */
      if ( (!search || !strcmp(search, "") ||
          (search && 
           (!strcasecmp(passwd[0], search) ||
            !strcasecmp(passwd[3], search) ||
            !strcasecmp(passwd[4], search) ||
            !strcasecmp(passwd[5], search)))) && atoi(passwd[2]) > 99) {
      /* Print entry */
      printf("<tr onmouseover=\"this.className='%s';\""
                  " onmouseout=\"this.className='%s';\">\n"
             "<td width=\"30\"><input type=\"checkbox\" name=\"user_%s\" value=\"checked\" /></td>\n"
             "<td width=\"60\"><a href=\"%s?command=detail&amp;id=%s\" />%s</td>\n"
             "<td width=\"90\">%s</td>\n"
             "<td width=\"120\">%s</td>\n"
             "<td width=\"150\">%s</td>\n"
             "<td width=\"120\">%s</td>\n"
             "<td width=\"80\">%s</td>\n"
             "</tr>\n",
             CONTENT_TABLE_CLASS_MOUSEOVER,
             CONTENT_TABLE_CLASS_MOUSEOUT,
             passwd[0],
             getenv("SCRIPT_NAME"),
             passwd[0],
             passwd[0],
             passwd[2],
             passwd[3],
             passwd[4],
             passwd[5],
             passwd[6]);
      }
      /* Increase counter */
      i++;
      /* Free passwd entry */
      argument_free(passwd);
   }
   /* Print table footer */
   printf("</tbody>\n"
          "</table>\n"
          "</div>\n"
          "</div>\n");
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

   /* Loop through all user entries in /etc/passwd */
   while ( i < file_line_counter) {
      /* Parse passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(passwd[0], username)) {
         printf("<form action=\"%s\" method=\"POST\">\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\">\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\">\n"
                "<table cellpadding=\"0\" cellspacing=\"10\">\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"user_password\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"user_password_check\" /></td>\n"
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
                "<td><input type=\"text\" name=\"user_gecos\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"user_directory\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"user_shell\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td colspan=\"2\">\n"
                "<input type=\"submit\" value=\"Update\">\n"
                "</td>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
                passwd[0],
                USER_TABLE_DESCRIPTION,
                passwd[0],
                USER_TABLE_NEW_PASSWORD,
                USER_TABLE_NEW_PASSWORD_CHECK,
                USER_TABLE_UID,
                passwd[2],
                USER_TABLE_GID,
                passwd[3],
                USER_TABLE_GECOS,
                passwd[4],
                USER_TABLE_DIRECTORY,
                passwd[5],
                USER_TABLE_SHELL,
                passwd[6]);


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
      owi_headline(2, "User not found");
   }
}

/* \fn user_update(username, password, gecos, directory, loginshell)
 * Update an entry in password file
 * \param[in] username username that will be updated
 * \param[in] password new password in cleartext (will be crypted)
 * \param[in] gecos new gecos field
 * \param[in] directory new home directory
 * \param[in] shell new shell
 */
void user_update(char *username, char *password, char *gecos, char *directory, char *shell) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(username, passwd[0])) {
         /* Set new passwd line in memory */
         file_line_action(FILE_LINE_SET, i,
                          "%s:%s:%s:%s:%s:%s:%s",
                          username,
                          crypt(password, "OM"),
                          passwd[2],
                          passwd[3],
                          gecos,
                          directory,
                          shell);
      }
      /* Free passwd entry */
      argument_free(passwd);
   }
   
   /* Save result in user file */
   file_save(USER_FILE);
}

/* \fn user_update(username, password, gecos, directory, loginshell)
 * Update an entry in password file
 * \param[in] username username that will be updated
 * \param[in] password new password in cleartext (will be crypted)
 * \param[in] gecos new gecos field
 * \param[in] directory new home directory
 * \param[in] shell new shell
 */
void user_add(char *username, char *password, char *gecos, char *directory, char *shell) {
   /* Index counter */
   int i;
   /* Max uid */
   unsigned int max_uid = 0;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **passwd = argument_parse(file_line_get(i), ":");
      /* Current uid greater than max uid? */
      if (atoi(passwd[2]) > max_uid) {
         /* Set max uid */
         max_uid = atoi(passwd[2]);
      }
      /* Free passwd entry */
      argument_free(passwd);
   }
   /* Increase max uid */
   max_uid++;
   /* Add new passwd line in memory */
   file_line_action(FILE_LINE_ADD, i,
                    "%s:%s:%d:%d:%s:%s:%s",
                    username,
                    crypt(password, "OM"),
                    max_uid,
                    max_uid,
                    gecos,
                    directory,
                    shell);

   /* Save result in user file */
   file_save(USER_FILE);
}
