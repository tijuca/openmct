/* read_form_data.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 *
 * some static html templates
 */

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "owi2.h"
#include "html.h"
#include "functions.h"

void page_head(char *title) {
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page head template started\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    printf("Content-Type: text/html\n\n");
    printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
    printf("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n");
    printf("<head><title>%s</title>",title);
    printf("<link rel=\"stylesheet\" type =\"text/css\" href=\"../css/default.css\">\n"
           "<link rel=\"stylesheet\" type =\"text/css\" href=\"../css/menu.css\">\n"
           "<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n"
           "<meta http-equiv=\"expires\" content=\"-1\" />\n"
           "<meta http-equiv= \"pragma\" content=\"no-cache\" />\n"
           "<meta name=\"author\" content=\"OMCT Team\" />\n"
           "<meta name=\"description\" content=\"!!!To fill with the right content!!!\" />\n"); //the 'content' has to become dynamic!!
    printf("</head>\n"
           "<body>\n");
    printf("<div id=\"top\">\n"
           "<h1 class=\"text_top\">Welcome on your OpenMCT NAS %s!</h1>\n",statistics.rev); // software rev number
    printf("<h2 class=\"text_top\">%s %s</h2>\n",statistics.vendor,statistics.cpu); // the vendor + model
    printf("<p class=\"text_top\"><img class=\"text_top\" src=\"../img/kfm_home.png\" border=0 ><b>Hostname:</b>   %s"
           "<img class=\"text_top\" src=\"../img/konquest.png\" border=0 ><b>Uptime:</b>   %s "
           "<img class=\"text_top\" src=\"../img/kcmmemory.png\" border=0 ><b>Memory:</b>   %s"
           "<img class=\"text_top\" src=\"../img/kcmprocessor.png\" border=0 ><b>CPU Load:</b> %s%% "
           "<img class=\"text_top\" src=\"../img/TuxKernel.48.png\" border=0 ><b>Kernel:</b>%s</p></div>\n"
           //,read_hostname()
			,statistics.hostname
			,statistics.uptime
			,get_memory()
			,get_loadavg()
			,statistics.kernel);

#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page head template ended\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
}

/* print the static menu lis */
void page_menu(){
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page menu template started\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    printf("<div id=\"left\">\n"
           "<div id=\"navcontainer\">\n"
           "<ul>\n"
           "    <li><a id=\"menu_first_level\"><img src=\"../img/preferences-system_16x16.png\"> System</a>\n"
           "    <ul id=\"menu_second_level\">\n"
           "        <li><a href=\"../cgi-bin/owi?modul=create-global-set\"><img src=\"../img/settings_16x16.gif\" border=0 >  global Settings</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/system-user_16x16.png\" border=0 >  User</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/system-users_16x16.png\" border=0 >  Groups</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/hd_16x16.png\" border=0 >  Harddisk</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/shellscript_16x16.png\" border=0 >  Shell</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/update_16x16.png\" border=0 >  Update/Backup</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/printer_16x16.png\" border=0 >  Printserver</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/usbdrive_16x16.png\" border=0 >  USB</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/time_16x16.png\" border=0 >  Timesettings</a></li>\n"

           "    </ul>\n");
    printf("    </li>\n"
           "        <li><a id=\"menu_first_level\"><img src=\"../img/network-global_16x16.png\"> Network</a>\n"
           "        <ul id=\"menu_second_level\">\n"
           "        <li><a href=\"../cgi-bin/owi?modul=interfaces\"><img src=\"../img/network_16x16.png\" border=0 >  Interfaces</a></li>\n"
           /*"        <li><a href=\"#\"><img src=\"../img/lock_16x16.png\" border=0 >  VPN</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/ftp_16x16.png\" border=0 >  FTP</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/nfs_16x16.png\" border=0 >  NFS</a></li>\n"
           "        <li><a href=\"#\"><img src=\"../img/samba_16x16.png\" border=0 >  Samba</a></li>\n"
           */
           "        </ul>\n"
           /*
           "    <li><a id=\"menu_first_level\" href=\"#\"><img src=\"../img/cancel.png\" border=0> too fill up</a></li>\n"
           "    <li><a id=\"menu_first_level\" href=\"#\"><img src=\"../img/warning_16x16.png\" border=0> also to fill up</a></li>\n"
           */
           "</ul>\n"
           "</div></div>\n");
#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page menu template ended\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif

}

void page_content_start() {
    printf("    <div id=\"right\">\n");
}

void page_content_end() {
    printf("    </div>\n");
}

void page_footer() {
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page footer template started\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    printf("    <div id=\"footer\"><p>\n"
           "        <b>OMCT WebIF </b> %s (%s) / Visit <a href=\"http://www.openmct.org\">www.openmct.org</a></b></p>\n"
           "    </div>\n",OWI_VERSION,__DATE__);
    printf("</body></html>\n");
#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page footer template ended\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
}
