/* -*- mode: C; c-file-style: "gnu" -*- */
/* group.c  group management
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
#include <grp.h>
#include <unistd.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/group.h"

/* \fn group_main(argc, argv)
 * Show all groups from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int group_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
   
   /* Print header information */
   owi_header("Group Management");
   
   /* Command NULL or empty? */
   if (!command || !strcmp(command, "")) {
      /* Just print user list */
      group_list();
   /* show user detail */
   } else if (!strcasecmp(command, "detail")) {
      /* Show user detail */
      group_detail(variable_get("id"));
   } else if (!strcasecmp(command, "update")) {
      group_update(variable_get("id"));
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn group_list()
 * Show all users from system
 */
void group_list() {
   /* variable for handling user entry in /etc/passwd */
   struct group *group;

   /* print headline */
   owi_headline(1, GROUP_HEADLINE);

   printf("<h1>%s</h1>\n", variable_get("andi"));
   
   /* print table headline */
   printf("<table cellpadding=\"0\" cellspacing=\"0\">\n"
          "<thead>\n"
          "<tr>\n"
          "<td></td>\n"
          "<td>%s</td>\n"
          "<td>%s</td>\n"
          "<td>%s</td>\n"
          "<td></td>\n"
          "</tr>\n"
          "<tbody>", 
          GROUP_TABLE_DESCRIPTION,
          GROUP_TABLE_GID,
          GROUP_TABLE_MEMBERS);
   /* Loop through all user entries in /etc/passwd */
   while ( (group = getgrent()) != NULL) {
      /* Print entry */
      printf("<tr onmouseover=\"this.style.backgroundColor='%s';\""
                  " onmouseout=\"this.style.backgroundColor='%s';\">\n"
             "<td><input type=\"checkbox\" name=\"group_%s\" value=\"checked\" /></td>\n"
             "<td>%s</td>\n"
             "<td>%d</td>\n"
             "<td>%s</td>\n"
             "<td><a href=\"%s?command=detail&id=%s\" />%s</td>\n"
             "</tr>\n",
             CONTENT_TABLE_CLASS_MOUSEOVER,
             CONTENT_TABLE_CLASS_MOUSEOUT,
             group->gr_name,
             group->gr_name,
             group->gr_gid,
             "",
             getenv("SCRIPT_NAME"),
             group->gr_name,
             GROUP_BUTTON_MODIFY);
   }
   /* Print table footer */
   printf("</tbody>\n"
          "<tfoot>\n"
          "<tr>\n"
          "<td colspan=\"8\" align=\"right\"><input class=\"button\""
          " type=\"button\" onClick=\"delete\" value=\"%s\" /></td>\n"
          "</tfoot>\n"
          "</table>\n",
          GROUP_BUTTON_DELETE);
   /* closes /etc/passwd */
   endgrent();
}

/* \fn group_detail(groupname)
 * Show one group account
 */
void group_detail(char *groupname) {
   /* variable for handling user entry in /etc/passwd */
   struct group *group;
   /* User found? */
   int group_found = 0;

   /* Loop through passwd database */
   while ( (group = getgrent()) != NULL) {
      /* Match found? */
      if (!strcmp(group->gr_name, groupname)) {
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
                "<td><input type=\"group_password\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"group_password_check\" /></td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%d</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "<tr>\n"
                "<td colspan=\"2\">\n"
                "<input type=\"submit\" value=\"Update\">\n"
                "</td>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
                group->gr_name,
                GROUP_TABLE_DESCRIPTION,
                group->gr_name,
                GROUP_TABLE_NEW_PASSWORD,
                GROUP_TABLE_NEW_PASSWORD_CHECK,
                GROUP_TABLE_GID,
                group->gr_gid,
                GROUP_TABLE_MEMBERS,
                "");


         /* Set user found to one */
         group_found = 1;
      }
   }
   /* closes /etc/passwd */
   endgrent();

   /* No user found? */
   if (!group_found) {
      /* Print information screen */
      owi_headline(2, "User not found");
   }
}

/* \fn group_update(groupname)
 * Update a group
 * \param[in] groupname group that will be updated
 */
void group_update(char *groupname) {
   /* Variable for handling group entry in /etc/group */
   struct group *group;
   /* File handling for writing password records */
   /* FILE *fp = fopen("/tmp/.group.tmp", "w"); */
   /* User found? */
   int group_found = 0;

   /* Loop through passwd database */
   while ( (group = getgrent()) != NULL) {
      /* Match found? */
      if (!strcmp(group->gr_name, groupname)) {
         group_found = 1;
      }
      /* putgrent(group, fp); */
   }
 
   /* closes /etc/passwd */
   endgrent();

   /* No user found? */
   if (!group_found) {
      /* Print information screen */
      owi_headline(2, "User not found");
   } else {
      unlink("/etc/group");
      rename("/tmp/.group.tmp", "/etc/group");
      /* Display user */	   
      group_detail(groupname);
   }
}
