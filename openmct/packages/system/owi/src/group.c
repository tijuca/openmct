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
#include <unistd.h>
#include <crypt.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/group.h"
#include "includes/file.h"

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
 
   /* Read file into memory */
   if (file_open(GROUP_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print user list */
         group_list();
      /* show user detail */
      } else if (!strcasecmp(command, "detail")) {
         /* Show user detail */
         group_detail(variable_get("id"));
      } else if (!strcasecmp(command, "update")) {
         group_update(variable_get("id"), variable_get("members"));
      } else if (!strcasecmp(command, "new")) {
         group_new(variable_get("id"));
      } else if (!strcasecmp(command, "add")) {
         group_add(variable_get("id"));
      } else if (!strcasecmp(command, "delete")) {
         group_delete(variable_get("id"));
      }
      /* Free file */
      file_free(GROUP_FILE);
   } else {
      /* Print error message */
      owi_headline(1, GROUP_HEADLINE);
      owi_headline(2, GROUP_FILE_FAILED);
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
   /* Index counter */
   int i = 0;	
   /* Search string */
   char *search = variable_get("search");

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
          "<table class=\"%s\" cellpadding=\"0\" cellspacing=\"0\">\n"
          "<thead>\n"
          "<tr>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "<th>%s</th>\n"
          "</tr>\n"
          "</thead>\n"
          "<tbody>",
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_CLASS,
          GROUP_HEADLINE,
          GROUP_DESCRIPTION,
          variable_get("search"),
          CONTENT_TABLE_LIST_CLASS,
          GROUP_TABLE_DESCRIPTION,
          GROUP_TABLE_GID,
          GROUP_TABLE_MEMBERS,
          GROUP_TABLE_ACTION);
   /* Loop through all user entries in group file */
   while ( i < file_line_counter) {
      char **group = argument_parse(file_line_get(i), ":");
      /* Search string specified? */
      if (!search || !strcmp(search, "") ||
          (search &&
           (strstr(argument_get_part(group, 0), search) ||
            strstr(argument_get_part(group, 1), search)))) {
         /* Print entry */
         printf("<tr onmouseover=\"this.style.backgroundColor='%s';\""
                     " onmouseout=\"this.style.backgroundColor='%s';\">\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "<td>%s</td>\n"
                "<td><input type=\"button\" onClick=\"location='%s?command=detail&id=%s'\" value=\"%s\" />&nbsp;<input type=\"button\" onClick=\"location='%s?command=delete&id=%s'\" value=\"%s\" /></td>\n"
                "</tr>\n",
                CONTENT_TABLE_CLASS_MOUSEOVER,
                CONTENT_TABLE_CLASS_MOUSEOUT,
                argument_get_part(group, 0),
                argument_get_part(group, 2),
                argument_get_part(group, 3),
                getenv("SCRIPT_NAME"),
                argument_get_part(group, 0),
                GROUP_BUTTON_MODIFY,
		getenv("SCRIPT_NAME"),
                argument_get_part(group, 0),
                GROUP_BUTTON_DELETE);
      }
      /* Increase index counter */
      i++;
      /* Free group entry */
      argument_free(group);
   }
   /* Print table footer / close internal table / close scroll / print footer / ... */
   printf("</tbody>\n"
          "</table>\n"
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
          GROUP_BUTTON_NEW);
}

/* \fn group_detail(groupname)
 * Show one group account
 */
void group_detail(char *groupname) {
   /* Index counter */
   int i = 0;	
   /* Group found? */
   int group_found = 0;

   /* Print external table for design */
   printf("<table class=\"%s\">\n"
          "<tr>\n"
          "<td>\n"
          "<h1>%s</h1>\n"
          "<br />%s<br /><br />\n",
          CONTENT_TABLE_CLASS,
          GROUP_HEADLINE,
          GROUP_DETAIL);

   /* Loop through passwd database */
   while ( i < file_line_counter) {
      /* Get group entry */	   
      char **group = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(group, 0), groupname)) {
         printf("<form action=\"%s\" method=\"POST\">\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\">\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\">\n"
                "<table class=\"%s\" width=\"100%%\">\n"
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
                "<td>%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td>%s</td>\n"
                "<td><input type=\"text\" name=\"members\" value=\"%s\" /></td>\n"
                "</tr>\n"
                "</table>\n"
                "<table width=\"100%%\">\n"
                "<tr>\n"
                "<td colspan=\"2\" align=\"right\">\n"
                "<input type=\"submit\" value=\"%s\" />\n"
                "</td>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
                argument_get_part(group, 0),
		CONTENT_TABLE_BOX_CLASS,
                GROUP_TABLE_DESCRIPTION,
                argument_get_part(group, 0),
                GROUP_TABLE_NEW_PASSWORD,
                GROUP_TABLE_GID,
                argument_get_part(group, 2),
                GROUP_TABLE_MEMBERS,
                argument_get_part(group, 3),
		GROUP_BUTTON_UPDATE);


         /* Set user found to one */
         group_found = 1;
      }
      /* Increase index counter */
      i++;
      /* Free group entry */
      argument_free(group);
   }

   /* No user found? */
   if (!group_found) {
      /* Print information screen */
      owi_headline(2, "Group not found");
   }

   /* Close external table */
   printf("</td>\n"
          "</tr>\n"
          "</table>\n");
}

/* \fn group_update(groupname, members)
 * Update an entry in password file
 * \param[in] groupname group that will be updated
 * \param[in] members comma seperated list of usernames
 */
void group_update(char *groupname, char *members) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(groupname, argument_get_part(group, 0))) {
         /* Set new passwd line in memory */
         file_line_action(FILE_LINE_SET, i,
                          "%s:%s:%s:%s",
                          argument_get_part(group, 0),
                          argument_get_part(group, 1),
                          argument_get_part(group, 2),
                          members);
      }
      /* Free passwd entry */
      argument_free(group);
   }

   /* Save result in user file */
   file_save(GROUP_FILE);

   /* Display group */
   group_detail(groupname);
}

/* \fn group_delete(username)
 * Delete an entry from group file
 * \param[in] groupname group that will be deleted
 */
void group_delete(char *groupname) {
   /* Index counter */
   unsigned int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get group entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(groupname, argument_get_part(group, 0))) {
         /* Delete group line in memory */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free group entry */
      argument_free(group);
   }

   /* Save result in user file */
   file_save(GROUP_FILE);

   /* Display user */
   group_list();
}

/* \fn group_add(groupname)
 * Execute a new group now
 * \param[in] groupname groupname that will be updated
 */
void group_add(char *groupname) {
   /* Index counter */
   int i;
   /* Max uid */
   unsigned int start_gid = 1000;
   /* Group already found? */
   int found = 0;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get group entry */
      char **group = argument_parse(file_line_get(i), ":");
      /* Current uid greater than max uid? */
      if (atoi(argument_get_part(group, 2)) == start_gid) {
         /* Set new start gid */
         start_gid++;
	 /* Start rescan */
	 i = 0;
      }
      /* Found? */
      if (!strcasecmp(groupname, argument_get_part(group, 0))) {
         /* Mark as found */
	 found = 1;
      }
      /* Free passwd entry */
      argument_free(group);
      /* Found? */
      if (found) {
         break;
      }
   }
   /* Not found? */
   if (!found) {
      /* Add new group line in memory */
      file_line_action(FILE_LINE_ADD, i,
                       "%s:%s:%d:%d:%s:%s:%s",
                       groupname,
                       crypt(variable_get("password"), "OM"),
                       start_gid,
                       variable_ltrim(variable_filter(variable_get("shell"), ":")));

      /* Save result in group file */
      file_save(GROUP_FILE);
   }

   /* Show all groups */
   group_detail(groupname);
}

/* \fn group_new()
 * Show the add screen for adding a group
 */
void group_new() {
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
          "<td><input type=\"text\" name=\"members\" /></td>\n"
          "</tr>\n"
          "</table>\n"
          "<table width=\"100%%\">\n"
          "<tr>\n"
          "<td colspan=\"2\" align=\"right\">\n"
          "<input type=\"submit\" value=\"%s\" />\n"
          "</td>\n"
          "</table>\n"
          "</form>\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n",
          CONTENT_TABLE_CLASS,
	  GROUP_HEADLINE,
	  GROUP_NEW,
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_BOX_CLASS,
          GROUP_TABLE_DESCRIPTION,
          GROUP_TABLE_NEW_PASSWORD,
          GROUP_TABLE_MEMBERS,
          GROUP_BUTTON_ADD);
}
