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
   owi_header("Network File System");

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
         nfs_update(variable_get("id"), variable_get("options"));
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

   owi_image("images/nfs.png", NFS_HEADLINE, NFS_HEADLINE);
   owi_headline(1, NFS_HEADLINE);
   owi_headline(2, NFS_DESCRIPTION);

   /* print table headline */
   printf("<div class=\"dataGridHeader\">\n"
          "<div class=\"dataGridContent\">\n"
          "<table class=\"%s\">\n"
          "<thead>\n"
          "<tr>\n"
          "<td width=\"30\">&nbsp;</td>\n"
          "<td width=\"120\">%s</td>\n"
          "<td width=\"500\">%s</td>\n"
          "<td></td>\n"
          "</tr>\n"
          "<tbody>",
          CONTENT_TABLE_CLASS,
          NFS_TABLE_DESCRIPTION,
          NFS_TABLE_OPTIONS);

   /* Loop through all nfs entries in nfs file */
   while ( i < file_line_counter) {
      /* Parse nfs entry */
      char **nfs = argument_parse(file_line_get(i), " 	");
      /* Search string specified? */
      if ((!search || !strcmp(search, "") ||
          (search && 
           (!strcasecmp(nfs[0], search) ||
            !strcasecmp(nfs[3], search) ||
            !strcasecmp(nfs[4], search) ||
            !strcasecmp(nfs[5], search)))) &&
	    (nfs[0][0] != '#')) {
      /* Print entry */
      printf("<tr onmouseover=\"this.className='%s';\""
                  " onmouseout=\"this.className='%s';\">\n"
             "<td width=\"30\"><input type=\"checkbox\" name=\"nfs_%s\" value=\"checked\" /></td>\n"
             "<td width=\"120\"><a href=\"%s?command=detail&amp;id=%s\" />%s</td>\n"
             "<td width=\"500\">%s</td>\n"
             "</tr>\n",
             CONTENT_TABLE_CLASS_MOUSEOVER,
             CONTENT_TABLE_CLASS_MOUSEOUT,
             nfs[0],
             getenv("SCRIPT_NAME"),
             nfs[0],
             nfs[0],
             nfs[1]);
      }
      /* Increase counter */
      i++;
      /* Free nfs entry */
      argument_free(nfs);
   }
   /* Print table footer */
   printf("</tbody>\n"
          "</table>\n"
          "</div>\n"
          "</div>\n");
}

/* \fn nfs_detail(nfsname)
 * Show one nfs account
 * \param[in] nfsname nfs that will be displayed
 */
void nfs_detail(char *nfsname) {
   /* NFS share found? */
   int nfs_found = 0;
   /* Index counter */
   int i = 0;

   /* Loop through all nfs entries in /etc/nfs */
   while ( i < file_line_counter) {
      /* Parse nfs entry */
      char **nfs = argument_parse(file_line_get(i), " 	");
      /* Match found? */
      if (!strcmp(nfs[0], nfsname)) {
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
                "<td><input type=\"nfs_options\" value=\"%s\"/></td>\n"
                "</tr>\n"
                "</table>\n"
                "</form>\n"
                ,
                getenv("SCRIPT_NAME"),
                nfs[0],
                NFS_TABLE_DESCRIPTION,
                nfs[0],
		NFS_TABLE_OPTIONS,
		nfs[1]);


         /* Set nfs found to one */
         nfs_found = 1;
      }
      /* Increase index counter */
      i++;
      /* Free nfs entry */
      argument_free(nfs);
   }

   /* No nfs found? */
   if (!nfs_found) {
      /* Print information screen */
      owi_headline(2, "NFS Share not found");
   }
}

/* \fn nfs_update(nfsname, options)
 * Update an entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 * \param[in] options new options for this share
 */
void nfs_update(char *nfsname, char *options) {
   /* Index counter */
   unsigned int i;

   /* Loop through nfs entries */
   for (i = 0; i < file_line_counter; i++) {
      /* Get nfs entry */
      char **nfs = argument_parse(file_line_get(i), ":");
      /* Passwd entry found? */
      if (!strcasecmp(nfsname, nfs[0])) {
         /* Set new nfs line in memory */
         file_line_action(FILE_LINE_SET, i,
                          "%s %s",
                          nfs[0],
                          options);
      }
      /* Free nfs entry */
      argument_free(nfs);
   }
   
   /* Save result in nfs file */
   file_save(NFS_FILE);
}

/* \fn nfs_add(nfsname, options)
 * Add a new entry in nfs file
 * \param[in] nfsname nfsname that will be updated
 * \param[in] options new options for this share
 */
void nfs_add(char *nfsname, char *options) {
   /* Add new nfs line in memory */
   file_line_action(FILE_LINE_ADD, file_line_counter,
                    "%s %s",
                    nfsname,
                    options);

   /* Save result in nfs file */
   file_save(NFS_FILE);
}
