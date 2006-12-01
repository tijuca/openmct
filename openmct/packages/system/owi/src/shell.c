/* -*- mode: C; c-file-style: "gnu" -*- */
/* shell.c User management
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
#include "includes/file.h"
#include "includes/shell.h"

/* \fn shell_main(argc, argv)
 * Show all shells from system
 * \param[in] argc command line argument counter
 * \param[in] argv character pointer array (arguments)
 * \return zero on sucess
 */
int shell_main(int argc, char **argv) {
   /* Index counter */
   int i;

   owi_header(SHELL_HEADLINE);

   /* Print external table for design */
   printf("<table width=\"%s\">\n"
          "<tr>\n"
          "<td>\n",
          CONTENT_TABLE_CLASS);

   /* Print headline information */
   owi_headline(1, SHELL_HEADLINE);
   printf("<br />%s<br /><br />\n", SHELL_DESCRIPTION);

   /* Print table head */
   printf("<form action=\"%s\" method=\"post\">\n"
          "<input type=\"hidden\" name=\"command\" value=\"execute\" />\n"
          "<table class=\"%s\" width=\"100%%\">\n"
	  "<tr>\n"
	  "<td>\n",
          getenv("SCRIPT_NAME"),
          CONTENT_TABLE_CLASS);

   /* No directory set? */
   if (!strcasecmp(variable_get("directory"), "")) {
      /* Set root directory as default */
      variable_set("directory", "/");
   }

   /* Change to directory before execting command */
   chdir(variable_get("directory"));

   printf("<textarea rows=20 cols=90 background=#000000>\n");
   /* Command NULL or empty? */
   if (!strcasecmp(variable_get("command"), "execute")) {
      char **cmd_argv = argument_parse(variable_get("value"), " 	");
      char curdir[1024];

      printf("%s # %s\n", variable_get("directory"), variable_get("value"));

      if (!strcasecmp(cmd_argv[0], "cd")) {
         chdir(cmd_argv[1]);
      } else {
         /* Execute command now */
         proc_open(variable_get("value"));
	 /* Loop through results */
         for (i = 0; i < file_line_counter; i++) {
            printf("%s\n", file_line_get(i));
         }
      }
      /* Get directory after executing command */
      getcwd(curdir, sizeof(curdir));
      /* Set new directory */
      variable_set("directory", curdir);
      printf("%s # ", variable_get("directory"));
   }
   printf("</textarea>\n");

   printf("</td>\n"
          "</tr>\n"
	  "<tr>\n"
	  "<tr>\n"
	  "<td>\n"
	  "<input type=\"hidden\" name=\"directory\" value=\"%s\" />\n"
	  "%s # <input type=\"text\" name=\"value\" size=\"100\"/>\n"
	  "</td>\n"
	  "</tr>\n"
	  "</table>\n"
	  "</form>\n",
	  variable_get("directory"),
	  variable_get("directory"));

   printf("<script type=\"text/javascript\">\n"
	  "document.forms[0].value.focus();\n"
	  "</script>\n");

   /* Print footer information */
   owi_footer();

   /* Return success */
   return 0;
}