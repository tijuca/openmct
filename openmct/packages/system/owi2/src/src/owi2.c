/* owi2.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 */

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "owi2.h"
#include "html.h"
#include "functions.h" //loadaverage

void printf_error(char *str) {
	printf("Content-Type: text/html\n\n");
	printf("<html><head>\n");
	printf("<title>CGI-Fehlermeldung</title>\n");
	printf("</head><body>\n");
	printf("%s",str);
	printf("</body></html>\n");
}

int main(void) {
    char *str;

    struct CGI_DATA *cgi;
    struct CGI_DATA *free_cgi;

    time_t today;
    time(&today);

#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "\n[%s %s/%d] %s %s started\n",__TIME__,__FUNCTION__,__LINE__,PACKAGE_NAME,OWI_VERSION);
    debug_info(debug_buf);
#endif
	// first we read the system infos
	if(get_sysinfos()!=0) {
		printf_error("Fehler beim Erstellen der Systeminfos");
		return 1;
	}
	// second we get the hd infos
	if(get_hd_info()<=0) {
		printf_error("Fehler beim Erstellen der HD Infos");
		return 1;
	}
	// reading in the rcconf parameter
    if(read_rcconf()==0) {
        printf_error("Fehler &#246ffnen rc.conf");
        return 1;
    }

    // reading data from the input
    str = getdata();
#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] readed string: '%s'\n",__FUNCTION__,__LINE__,str);
    debug_info(debug_buf);
#endif
/* FIXMe! maybe someone call the owi without any given modul! */
	/*if (strcmp(str,"")==0) {
		free_cgi = cgi;
		start_page("Startseite/Uebersicht");
		printf_error("Fehler");
	}*/
    if(str == NULL) {
	   snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] uuuhh, something wrong while parsing form data!\n",__FUNCTION__,__LINE__);
       debug_info(debug_buf);
	   return EXIT_FAILURE;
    }
    // converting hexstrings to ASCII, move '+' to space
    hex2ascii(str);

    // create a list of readed data
    cgi = create_cgi_list(str);
    free_cgi = cgi;

    if (cgi == NULL) {
		printf_error("Fehler beim Erstellen der Variablen/Werte-Liste\n");
		return EXIT_FAILURE;
	}
    // now we have parsed all CGI data, we check now what page is requested

    // if Modul == start, we build the startpage
    if( ((strcmp(cgi->variable,"modul"))==0) && (strcmp(cgi->value,"start")==0) ) {
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] start creating 'start.html'\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
        start_page("Startseite/Uebersicht");
    } // if modul==create-global-set, we build the global settings page
    else if ( ((strcmp(cgi->variable,"modul"))==0) && (strcmp(cgi->value,"create-global-set")==0) ) {
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] start creating 'global-set.html'\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
        glob_set("globale Einstellungen");
    } // if modul==write-global-set, we write the new config rc.conf

    else if ( ((strcmp(cgi->variable,"modul"))==0) && (strcmp(cgi->value,"write-global-set")==0) ) {
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] start creating new 'rc.conf.new'\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
        //printf_error("Zur Zeit noch nicht moeglich!");
		glob_set("globale Einstellungen");
        if(write_rcconf(cgi)==0) {
			snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] creating of 'rc.conf.new successful!'\n",__FUNCTION__,__LINE__);
			debug_info(debug_buf);
        } else {
        	snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] creating of 'rc.conf.new NOT successful!'\n",__FUNCTION__,__LINE__);
			debug_info(debug_buf);
        }
    }
    else {
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] value of cgi->variable: %s cgi->value: %s\n",__FUNCTION__,__LINE__,cgi->variable,cgi->value);
        debug_info(debug_buf);
    }

	// Speicher wieder freigeben */
	delete(free_cgi);
#ifdef DEBUG
//    char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s %s/%d] %s %s end\n",__TIME__,__FUNCTION__,__LINE__,PACKAGE_NAME,OWI_VERSION);
    debug_info(debug_buf);
#endif
	return EXIT_SUCCESS;
}
