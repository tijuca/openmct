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
#include "includes/misc.h"

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
          GROUP_HEADLINE,
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          GROUP_TABLE_DESCRIPTION,
          GROUP_TABLE_MEMBERS,
          GROUP_TABLE_ACTION);
   /* Loop through all user entries in group file */
   while ( i < file_line_counter) {
      char **group = argument_parse(file_line_get(i), ":");
      /* Print entry */
      printf("<tr onmouseover=\"this.className='mover';\""
             " onmouseout=\"this.className='mout';\">\n"
             "<td width=\"200\">%s</td>\n"
             "<td width=\"212\">%s</td>\n"
             "<td width=\"100\"><input type=\"button\" onClick=\"location='%s?module=%s&command=detail&id=%s'\" value=\"%s\" />&nbsp;<input type=\"button\" onClick=\"location='%s?module=%s&amp;command=delete&amp;id=%s'\" value=\"%s\" /></td>\n"
             "</tr>\n",
             argument_get_part(group, 0),
             argument_get_part(group, 3),
             getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(group, 0),
             GROUP_BUTTON_MODIFY,
             getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(group, 0),
             GROUP_BUTTON_DELETE);
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
	  "<input type=\"button\" onClick=\"location='%s?module=%s&command=new'\" value=\"%s\" />\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n"
          "</form>\n",
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          GROUP_BUTTON_NEW);
}

/* \fn group_detail(groupname)
 * Show one group account
 */
void group_detail(char *groupname) {
   /* Index counter */
   int i = 0;
   /* Group */
   int group_found = 0;

   /* Print external table for design */
   printf("<h3>%s</h3>\n",
          GROUP_HEADLINE);

   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"error\">%s</div>\n",
             variable_get("error"));
   }

   /* Loop through passwd database */
   while ( i < file_line_counter) {
      /* Get group entry */	   
      char **group = argument_parse(file_line_get(i), ":");
      /* Match found? */
      if (!strcmp(argument_get_part(group, 0), groupname)) {
         printf("<form action=\"%s\" method=\"POST\">\n"
                "<table class=\"outside\">\n"
                "<tr>\n"
                "<td>\n"
                "<input type=\"hidden\" name=\"module\" value=\"%s\">\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\">\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\">\n"
                "<table class=\"detail\">\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\">%s</td>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"text\" name=\"members\" value=\"%s\" /><br />%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td></td>\n"
	        "<td><input type=\"button\" onClick=\"javascript:document.forms[0].submit()\" value=\"%s\" />\n"
                "</td>\n"
		"</tr>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
		variable_get("module"),
                argument_get_part(group, 0),
                GROUP_TABLE_DESCRIPTION,
                argument_get_part(group, 0),
                GROUP_TABLE_MEMBERS,
                argument_get_part(group, 3),
		GROUP_MEMBERS_DESCRIPTION,
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
      owi_headline(2, GROUP_NOT_FOUND);
   }

   /* Close external table */
   printf("</td>\n"
          "</tr>\n"
          "</table>\n");
}

/* \fn group_update(groupname, members)
 * Update an entry in group file
 * \param[in] groupname group that will be updated
 * \param[in] members comma seperated list of usernames
 */
void group_update(char *groupname, char *members) {
   /* Index counter */
   unsigned int i;
   /* Error */
   char *error = NULL;

   /* Invalid syntax for user description */
   if (!match(members, GROUP_MEMBERS_VALID)) {
      error = GROUP_MEMBERS_INVALID;
   }

   /* No error? */
   if (!error) {
      /* Loop through group entries */
      for (i = 0; i < file_line_counter; i++) {
         /* Get group entry */
         char **group = argument_parse(file_line_get(i), ":");
         /* Passwd entry found? */
         if (!strcasecmp(groupname, argument_get_part(group, 0))) {
            /* Set new group line in memory */
            file_line_action(FILE_LINE_SET, i,
                             "%s:%s:%s:%s",
                             argument_get_part(group, 0),
                             argument_get_part(group, 1),
                             argument_get_part(group, 2),
                             members);
         }
         /* Free group entry */
         argument_free(group);
      }

      /* Save result in user file */
      file_save(GROUP_FILE);

      /* Display group list */
      group_list();
   } else {
      variable_set("error", error);
      group_detail(groupname);
   }
}

/* \fn group_delete(username)
 * Delete an entry from group file
 * \param[in] groupname group that will be deleted
 */
void group_delete(char *groupname) {
   /* Index counter */
   unsigned int i;

   /* Loop through group entries */
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

   /* Display group list */
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
   /* Error */
   char *error = NULL;

   /* Loop through group entries */
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
	 error = GROUP_ALREADY_EXISTS;
      }
      /* Free group entry */
      argument_free(group);
      /* Found? */
      if (error) {
         break;
      }
   }

   if (!match(groupname, GROUP_VALID)) {
      error = GROUP_INVALID;
   } else if (!match(variable_get("members"), GROUP_MEMBERS_VALID)) {
      error = GROUP_MEMBERS_INVALID;
   }

   /* Not found? */
   if (!error) {
      /* Add new group line in memory */
      file_line_action(FILE_LINE_ADD, i,
                       "%s:x:%d:%s",
                       groupname,
                       start_gid,
                       variable_ltrim(variable_get("members")));

      /* Save result in group file */
      file_save(GROUP_FILE);

      /* Show all groups */
      group_list();
   } else {
      /* Assign template */
      variable_set("error", error);
      /* Display add screen */
      group_new();
   }
}

/* \fn group_new()
 * Show the add screen for adding a group
 */
void group_new() {
   /* Print external table for design */
   printf("<h3>%s</h3>\n",
          GROUP_HEADLINE);

   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"error\">%s</div>\n",
             variable_get("error"));
   }

   printf("<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
          "<input type=\"hidden\" name=\"command\" value=\"add\" />\n"
          "<table class=\"detail\">\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"text\" name=\"id\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"text\" name=\"members\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td></td>\n"
	  "<td>\n"
	  "<input type=\"button\" onClick=\"javascript:document.forms[0].submit()\" value=\"%s\" />\n"
          "</td>\n"
          "</table>\n"
          "</form>\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n",
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          GROUP_TABLE_DESCRIPTION,
	  variable_get("id"),
	  GROUP_DESCRIPTION,
          GROUP_TABLE_MEMBERS,
	  variable_get("members"),
	  GROUP_MEMBERS_DESCRIPTION,
          GROUP_BUTTON_ADD);
}
