/* -*- mode: C; c-file-style: "gnu" -*- */
/* nfs.c NFS (Network file system) management
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
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/file.h"
#include "includes/owi.h"
#include "includes/nfs.h"
#include "includes/misc.h"

/* \fn nfs_main(argc, argv)
 * Show all nfs shares from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int nfs_main(int argc, char **argv) {
   /* Get command for this module */        
   char *command = variable_get("command");
 
   /* Print header information */
   owi_header(NFS_HEADLINE);

   /* Read file into memory */
   if (file_open(NFS_FILE) != -1) {
      /* Command NULL or empty? */
      if (!command || !strcmp(command, "")) {
         /* Just print nfs list */
         nfs_list();
      /* show nfs detail */
      } else if (!strcasecmp(command, "detail")) {
         /* Show nfs detail */
         nfs_detail(variable_get("id"));
      } else if (!strcasecmp(command, "update")) {
         nfs_update(variable_get("id"));
	 /* Reloade NFS configuration */
         proc_open("exportfs -rfa");
      } else if (!strcasecmp(command, "delete")) {
         nfs_delete(variable_get("id"));
      } else if (!strcasecmp(command, "new")) {
         nfs_new();
      } else if (!strcasecmp(command, "add")) {
         nfs_add(variable_get("id"));
      }
      /* Free file */
      file_free();
   } else {
      /* Print error message */
      owi_headline(1, NFS_HEADLINE);
      owi_headline(2, NFS_FILE_FAILED);
   }

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}

/* \fn nfs_list()
 * Show all nfss from system
 */
void nfs_list() {
   /* Index counter */
   int i = 0;

   /* Start form / external table / scroll area / internal table*/
   printf("<h3>%s</h3\n"
          "<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
          "<input type=\"hidden\" name=\"command\" value=\"\" />\n"
          "<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n"
          "<table class=\"list\">\n"
	  "<thead>\n"
          "<tr>\n"
          "<th width=\"80\">%s</th>\n"
          "<th width=\"320\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "</tr>\n"
          "</thead>\n"
          "<tbody>\n",
          NFS_HEADLINE,
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          NFS_TABLE_DESCRIPTION,
          NFS_TABLE_SOURCE,
          NFS_TABLE_OPTIONS,
          NFS_TABLE_ACTION);

   /* Loop through all nfs entries in nfs file */
   for (i = 0; i < file_line_counter; i++) {
      /* Parse nfs entry */
      char **nfs = NULL;
      char **properties = NULL;
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      properties = argument_parse(argument_get(nfs, 1, " "), "()");
      /* Print entry */
      printf("<tr onmouseover=\"this.className='mover';\""
             " onmouseout=\"this.className='mout';\">\n"
             "<td width=\"80\">%s</td>\n"
             "<td width=\"320\">%s</td>\n"
             "<td width=\"160\">%s</td>\n"
   	     "<td width=\"160\">\n"
	     "<input type=\"button\" onClick=\"location='%s?module=%s&command=detail&amp;id=%s'\" value=\"%s\" />&nbsp;"
             "<input type=\"button\" onClick=\"location='%s?module=%s&command=delete&amp;id=%s'\" value=\"%s\" />"
             "</td>\n"
             "</tr>\n",
             argument_get_part(nfs, 0),
	     argument_get_part(properties, 0),
	     argument_get_part(properties, 1),
	     getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(nfs, 0),
	     NFS_BUTTON_MODIFY,
  	     getenv("SCRIPT_NAME"),
             variable_get("module"),
             argument_get_part(nfs, 0),
             NFS_BUTTON_DELETE);
      /* Free nfs entry */
      argument_free(nfs);
   }
   /* Print table footer / close internal table / close scroll / print footer / ... */
   printf("</tbody>\n"
          "</table>\n"
	  "<table width=\"100%%\">\n"
          "<tr>\n"
          "<td align=\"right\"><input type=\"button\" onClick=\"location='%s?module=%s&command=new'\" value=\"%s\" />"
          "</td>\n"
          "</tr>\n"
          "</table>\n"
          "</td>\n"
          "</tr>\n"
          "</table>\n"
          "</form>\n",
          getenv("SCRIPT_NAME"),
	  variable_get("module"),
          NFS_BUTTON_NEW);
}

/* \fn nfs_detail(nfsname)
 * Show one user account
 * \param[in] nfsname user that will be displayed
 */
void nfs_detail(char *nfsname) {
   /* User found? */
   int nfs_found = 0;
   /* Index counter */
   int i = 0;

   /* Print external table for design */
   printf("<h3>%s</h3>\n",
          NFS_HEADLINE);

   if (strcmp(variable_get("error"), "")) {
      printf("<div class=\"error\">%s</div>\n",
             variable_get("error"));
   }

   printf("<table class=\"outside\">\n"
          "<tr>\n"
          "<td>\n");

   /* Loop through all user entries in /etc/nfs */
   for (i = 0; i < file_line_counter; i++) {
      char **nfs = NULL;
      if (file_line_get(i)[0] == '#') {
         continue;
      }
      /* Parse nfs entry */
      nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* Match found? */
      if (!strcmp(argument_get_part(nfs, 0), nfsname)) {
         /* Get options data for this share */
         char **properties = argument_parse(argument_get(nfs, 1, " "), "()");
         /* Print data */
         printf("<form action=\"%s\" method=\"post\">\n"
                "<input type=\"hidden\" name=\"module\" value=\"%s\" />\n"
                "<input type=\"hidden\" name=\"command\" value=\"update\" />\n"
                "<input type=\"hidden\" name=\"id\" value=\"%s\" />\n"
                "<table class=\"detail\">\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\">%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"text\" name=\"source\" value=\"%s\" size=\"60\" /><br />%s</td>\n"
                "</tr>\n"
                "<tr>\n"
                "<td class=\"description\">%s</td>\n"
                "<td class=\"value\"><input type=\"text\" name=\"options\" value=\"%s\" size=\"60\" /><br />%s</td>\n"
                "</tr>\n"
                "</table>\n"
                "<table width=\"100%%\">\n"
                "<tr>\n"
                "<td colspan=\"2\" align=\"right\">\n"
                "<input type=\"submit\" value=\"%s\" />\n"
                "</td>\n"
                "</table>\n"
                "</form>\n",
                getenv("SCRIPT_NAME"),
		variable_get("module"),
                argument_get_part(nfs, 0),
                NFS_TABLE_DESCRIPTION,
                argument_get_part(nfs, 0),
                NFS_TABLE_SOURCE,
                argument_get_part(properties, 0),
		NFS_SOURCE_DESCRIPTION,
		NFS_TABLE_OPTIONS,
		argument_get_part(properties, 1),
		NFS_OPTIONS_DESCRIPTION,
		NFS_BUTTON_UPDATE);
         /* Free options data */
         free(properties);
         /* Set user found to one */
         nfs_found = 1;
      }
      /* Free nfs entry */
      argument_free(nfs);
   }

   /* No user found? */
   if (!nfs_found) {
      /* Print information screen */
      owi_headline(2, NFS_NOT_FOUND);
   }

   /* Close external table */
   printf("</td>\n"
          "</tr>\n"
	  "</table>\n");
}

/* \fn nfs_update(nfsname)
 * Update an entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 */
void nfs_update(char *nfsname) {
   /* Index counter */
   int i;
   /* Source value */
   char *source = variable_get("source");
   /* Options value */
   char *options = variable_get("options");
   /* Error */
   char *error = NULL;

   /* Invalid syntax for user description */
   if (!match(variable_get("source"), NFS_SOURCE_VALID)) {
      error = NFS_SOURCE_INVALID;
   } else if (!match(variable_get("options"), NFS_OPTIONS_VALID)) {
      error = NFS_OPTIONS_INVALID;
   }

   /* No error occured? */
   if (!error) {
      /* Loop through nfs entries */
      for (i = 0; i < file_line_counter; i++) {
         char **nfs = NULL;
         if (file_line_get(i)[0] == '#') {
            continue;
         }
         /* Parse nfs entry */
         nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
         /* Passwd entry found? */
         if (!strcasecmp(nfsname, argument_get_part(nfs, 0))) {
            /* Set new nfs line in memory */
            file_line_action(FILE_LINE_SET, i,
                             "%s %s(%s) %s",
                             argument_get_part(nfs, 0),
                             source,
                             options,
                             argument_get(nfs, 2, ARGUMENT_SEPERATOR_STANDARD));
         }
         /* Free nfs entry */
         argument_free(nfs);
      }
      /* Save result in nfs file */
      file_save(NFS_FILE);

      /* Display list */
      nfs_list();
   } else {
      variable_set("error", error);
      nfs_detail(nfsname);
   }
}

/* \fn nfs_add(nfsname)
 * Add a new entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 */
void nfs_add(char *nfsname) {
   /* Error */
   char *error = NULL;
   /* Counter */
   int i;

   /* Loop through passwd entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get passwd entry */
      char **nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* New share already in nfs database */
      if (!strcasecmp(argument_get_part(nfs, 0),
          nfsname)) {
         /* Set error to already exists */
         error = NFS_ALREADY_EXISTS;
      }
      /* Free passwd entry */
      argument_free(nfs);
      /* Found? */
      if (error) {
         break;
      }
   }

   if (!match(nfsname, NFS_VALID)) {
      error = NFS_INVALID;
   } else if (!match(variable_get("source"), NFS_SOURCE_VALID)) {
      error = NFS_SOURCE_INVALID;
   } else if (!match(variable_get("options"), NFS_OPTIONS_VALID)) {
      error = NFS_OPTIONS_INVALID;
   }

   if (!error) {
      /* Add new nfs line in memory */
      file_line_action(FILE_LINE_ADD, file_line_counter,
                       "%s %s(%s)",
                       nfsname,
                       variable_get("source"),
                       variable_get("options"));

      /* Save result in nfs file */
      file_save(NFS_FILE);

      /* Display all nfs shares */
      nfs_list();
   } else {
      variable_set("error", error);
      nfs_new();
   } 
}

/* \fn nfs_delete(nfsname)
 * Delete an entry from password file
 * \param[in] nfsname nfsname that will be deleted
 */
void nfs_delete(char *nfsname) {
   /* Index counter */
   unsigned int i;

   /* Loop through nfs entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* Passwd entry found? */
      if (!strcasecmp(nfsname, argument_get_part(nfs, 0))) {
         /* Set new nfs line in memory */
         file_line_action(FILE_LINE_DEL, i, NULL);
      }
      /* Free nfs entry */
      argument_free(nfs);
   }

   /* Save result in user file */
   file_save(NFS_FILE);

   /* Display all nfs shares */
   nfs_list();
}

/* \fn nfs_new()
 * Show the add screen for adding a user 
 */
void nfs_new() {
   /* Print external table for design */
   printf("<h3>%s</h3>",
          NFS_HEADLINE);

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
          "<td class=\"value\"><input type=\"text\" name=\"source\" value=\"%s\" /><br />%s</td>\n"
          "</tr>\n"
          "<tr>\n"
          "<td class=\"description\">%s</td>\n"
          "<td class=\"value\"><input type=\"text\" name=\"options\" value=\"%s\" /><br />%s</td>\n"
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
          NFS_TABLE_DESCRIPTION,
          variable_get("id"),
          NFS_DESCRIPTION,
          NFS_TABLE_SOURCE,
          variable_get("source"),
          NFS_SOURCE_DESCRIPTION,
          NFS_TABLE_OPTIONS,
          !strcmp(variable_get("options"), "") ? NFS_OPTIONS_DEFAULT : variable_get("options"),
          NFS_OPTIONS_DESCRIPTION,
          NFS_BUTTON_ADD);
}
