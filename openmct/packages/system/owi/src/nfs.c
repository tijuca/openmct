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
#include <crypt.h>
#include "includes/argument.h"
#include "includes/language.h"
#include "includes/template.h"
#include "includes/variable.h"
#include "includes/owi.h"
#include "includes/nfs.h"
#include "includes/file.h"

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
      file_free(NFS_FILE);
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
          "<input type=\"text\" name=\"search\" value=\"%s\" />&nbsp;"
          "<input type=\"submit\" value=\"Suchen\" /></td>\n"
          "</tr>\n"
          "</table>\n"
          "<table class=\"%s\" cellpadding=\"0\" cellspacing=\"0\">\n"
          "<thead>\n"
          "<tr>\n"
          "<th width=\"80\">%s</th>\n"
          "<th width=\"480\">%s</th>\n"
          "<th width=\"160\">%s</th>\n"
          "</tr>\n"
          "</thead>\n"
          "<tbody>",
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_CLASS,
          NFS_HEADLINE,
          NFS_DESCRIPTION,
          variable_get("search"),
          CONTENT_TABLE_LIST_CLASS,
          NFS_TABLE_DESCRIPTION,
          NFS_TABLE_OPTIONS,
          NFS_TABLE_ACTION);

   /* Loop through all nfs entries in nfs file */
   while ( i < file_line_counter) {
      /* Parse nfs entry */
      char **nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* Get options data for this share */
      char *options = NULL;
      /* Valid line? */
      if (argument_get_part(nfs, 0)[0] != '#') {
         options = argument_get(nfs, 1, ARGUMENT_SEPERATOR_STANDARD);
      }
      /* Everything ok? */
      if (options) {
         /* Search string specified? */
         if ((!search || !strcmp(search, "") ||
             (search && 
              (strstr(argument_get_part(nfs, 0), search) ||
               strstr(options, search))))) {
            /* Print entry */
            printf("<tr onmouseover=\"this.className='%s';\""
                     " onmouseout=\"this.className='%s';\">\n"
                   "<td width=\"80\">%s</td>\n"
                   "<td width=\"480\">%s</td>\n"
   	           "<td width=\"160\">\n"
	           "<input type=\"button\" onClick=\"location='%s?command=detail&amp;id=%s'\" value=\"%s\" />&nbsp;"
                   "<input type=\"button\" onClick=\"location='%s?command=delete&amp;id=%s'\" value=\"%s\" />"
                   "</td>\n"
                   "</tr>\n",
                   CONTENT_TABLE_CLASS_MOUSEOVER,
                   CONTENT_TABLE_CLASS_MOUSEOUT,
                   argument_get_part(nfs, 0),
                   options,
	           getenv("SCRIPT_NAME"),
                   argument_get_part(nfs, 0),
	           NFS_BUTTON_MODIFY,
  	           getenv("SCRIPT_NAME"),
                   argument_get_part(nfs, 0),
  	           NFS_BUTTON_DELETE);
	 }
      }
      if (options) {
         /* Free options */
         free(options);
      }	 
      /* Increase counter */
      i++;
      /* Free nfs entry */
      argument_free(nfs);
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
   printf("<table class=\"%s\">\n"
          "<tr>\n"
          "<td>\n"
	  "<h1>%s</h1>\n"
	  "<br />%s<br /><br />\n",
          CONTENT_TABLE_CLASS,
	  NFS_HEADLINE,
	  NFS_DETAIL);

   /* Loop through all user entries in /etc/nfs */
   while ( i < file_line_counter) {
      /* Parse nfs entry */
      char **nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* Match found? */
      if (!strcmp(argument_get_part(nfs, 0), nfsname)) {
         /* Get options data for this share */
         char *options = argument_get(nfs, 1, ARGUMENT_SEPERATOR_STANDARD);
         /* Everything ok? */
         if (options) {
            /* Print data */
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
                   "<td><input type=\"text\" name=\"options\" value=\"%s\" size=\"60\" /></td>\n"
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
                   argument_get_part(nfs, 0),
   	   	   CONTENT_TABLE_BOX_CLASS,
                   NFS_TABLE_DESCRIPTION,
                   argument_get_part(nfs, 0),
                   NFS_TABLE_OPTIONS,
                   options,
		   NFS_BUTTON_UPDATE);
            /* Free options data */
            free(options);
            /* Set user found to one */
            nfs_found = 1;
         }
      }
      /* Increase index counter */
      i++;
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

/* \fn nfs_update(nfsname, options)
 * Update an entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 */
void nfs_update(char *nfsname) {
   /* Index counter */
   int i;
   /* Options value */
   char *options = variable_get("options");

   /* Loop through nfs entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **nfs = argument_parse(file_line_get(i), ARGUMENT_SEPERATOR_STANDARD);
      /* Passwd entry found? */
      if (!strcasecmp(nfsname, argument_get_part(nfs, 0))) {
         /* Set new nfs line in memory */
         file_line_action(FILE_LINE_SET, i,
                          "%s %s",
                          argument_get_part(nfs, 0),
                          options);
      }
      /* Free nfs entry */
      argument_free(nfs);
   }
   
   /* Save result in nfs file */
   file_save(NFS_FILE);

   /* Display it */
   nfs_detail(nfsname);
}

/* \fn nfs_add(nfsname)
 * Add a new entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 */
void nfs_add(char *nfsname) {
   /* Options value */
   char *options = variable_get("options");

   /* Add new nfs line in memory */
   file_line_action(FILE_LINE_ADD, file_line_counter,
                    "%s %s",
                    nfsname,
                    options);

   /* Save result in nfs file */
   file_save(NFS_FILE);

   /* Display all nfs shares */
   nfs_list();
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
          "<td><input type=\"text\" name=\"options\" /></td>\n"
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
	  NFS_HEADLINE,
	  NFS_NEW,
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_BOX_CLASS,
          NFS_TABLE_DESCRIPTION,
          NFS_TABLE_OPTIONS,
          NFS_BUTTON_ADD);
}
