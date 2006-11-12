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
#include <pwd.h>
#include <unistd.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/user.h"

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
   /* variable for handling user entry in /etc/passwd */
   struct passwd *user;
   /* Get command for this module */        
   char *search = variable_get("search");
   /* counter for users */
   int user_count = 0;

   /* Loop through all user entries in /etc/passwd */
   while ( (user = getpwent()) != NULL) {
      user_count++;
   }
   /* Set pointer to start */
   setpwent();
   
   owi_image("images/user.png", USER_HEADLINE, USER_HEADLINE);
   owi_headline(1, USER_HEADLINE);
   owi_headline(2, USER_DESCRIPTION);

   printf("<table>\n"
          "<tr>\n"
          "<td>Seiten (%d):\n"
          "</td>\n"
          "<td align=\"right\"><input type=\"text\" name=\"search=\" /></td>\n"
          "</tr>\n"
          "</table>\n", 0);

   
   /* print table headline */
   printf("<table class=\"%s\">\n"
          "<thead>\n"
          "<tr>\n"
          "<td width=\"30\">&nbsp;</td>\n"
          "<td width=\"60\">%s</td>\n"
          "<td width=\"60\">%s</td>\n"
          "<td width=\"60\">%s</td>\n"
          "<td width=\"120\">%s</td>\n"
          "<td width=\"100\">%s</td>\n"
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

   /* Loop through all user entries in /etc/passwd */
   while ( (user = getpwent()) != NULL) {
      if (!search || !strcmp(search, "") ||
          (search && 
           (!strcasecmp(user->pw_name, search) ||
            !strcasecmp(user->pw_gecos, search) ||
            !strcasecmp(user->pw_dir, search) ||
            !strcasecmp(user->pw_shell, search)))) {
      /* Print entry */
      printf("<tr onmouseover=\"this.className='%s';\""
                  " onmouseout=\"this.className='%s';\">\n"
             "<td width=\"30\"><input type=\"checkbox\" name=\"user_%s\" value=\"checked\" /></td>\n"
             "<td width=\"60\"><a href=\"%s?command=detail&amp;id=%s\" />%s</td>\n"
             "<td width=\"60\">%d</td>\n"
             "<td width=\"60\">%d</td>\n"
             "<td width=\"120\">%s</td>\n"
             "<td width=\"100\">%s</td>\n"
             "<td width=\"80\">%s</td>\n"
             "</tr>\n",
             CONTENT_TABLE_CLASS_MOUSEOVER,
             CONTENT_TABLE_CLASS_MOUSEOUT,
             user->pw_name,
             getenv("SCRIPT_NAME"),
             user->pw_name,
             user->pw_name,
             user->pw_uid,
             user->pw_gid,
             user->pw_gecos,
             user->pw_dir,
             user->pw_shell);
      }   
   }
   /* Print table footer */
   printf("</tbody>\n"
          "</table>\n");
   /* closes /etc/passwd */
   endpwent();
}

/* \fn user_detail(username)
 * Show one user account
 * \param[in] username user that will be displayed
 */
void user_detail(char *username) {
   /* variable for handling user entry in /etc/passwd */
   struct passwd *user;
   /* User found? */
   int user_found = 0;

   /* Loop through passwd database */
   while ( (user = getpwent()) != NULL) {
      /* Match found? */
      if (!strcmp(user->pw_name, username)) {
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
                "<td>%d</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%d</td>\n"
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
                user->pw_name,
                USER_TABLE_DESCRIPTION,
                user->pw_name,
                USER_TABLE_NEW_PASSWORD,
                USER_TABLE_NEW_PASSWORD_CHECK,
                USER_TABLE_UID,
                user->pw_uid,
                USER_TABLE_GID,
                user->pw_gid,
                USER_TABLE_GECOS,
                user->pw_gecos,
                USER_TABLE_DIRECTORY,
                user->pw_dir,
                USER_TABLE_SHELL,
                user->pw_shell);


         /* Set user found to one */
         user_found = 1;
      }
   }
   /* closes /etc/passwd */
   endpwent();

   /* No user found? */
   if (!user_found) {
      /* Print information screen */
      owi_headline(2, "User not found");
   }
}

/* \fn user_update(username)
 * Update an user
 * \param[in] username user that will be updated
 */
void user_update(char *username) {
   /* Variable for handling user entry in /etc/passwd */
   struct passwd *user;
   /* File handling for writing password records */
   FILE *fp = fopen("/tmp/.passwd.tmp", "w");
   /* User found? */
   int user_found = 0;

   /* Loop through passwd database */
   while ( (user = getpwent()) != NULL) {
      /* Match found? */
      if (!strcmp(user->pw_name, username)) {
         user->pw_shell = variable_get("user_shell");
         user_found = 1;
      }
      putpwent(user, fp);
   }
 
   /* closes /etc/passwd */
   endpwent();

   /* No user found? */
   if (!user_found) {
      /* Print information screen */
      owi_headline(2, "User not found");
   } else {
      unlink("/etc/passwd");
      rename("/tmp/.passwd.tmp", "/etc/passwd");
      /* Display user */	   
      user_detail(username);
   }
}
