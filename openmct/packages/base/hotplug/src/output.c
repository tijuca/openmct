/* output.c
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)

this file is part of the hotplug handler for OpenMCT (http://www.openmct.org)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hotplug.h"

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

void write_debug_output(char *source, char *action, char *product, char *interface, char *device, char *type) {
	
    time_t today;
    time(&today);
/* if we want some output to the console */
#ifdef DEBUG
    FILE *debug = fopen("/dev/console", "w");
    fprintf(debug, "\n\n   Output enviroments given by the kernel ...\n");
    fprintf(debug, "\n##HOTPLUG EVENT##\n"
                 "           date: %s"
                 "         source: %s\n"
                 "         action: %s\n"
                 "        product: %s\n"
                 "      interface: %s\n"
                 "         device: %s\n"
                 "class/subclass/protokoll: %s\n\n", ctime(&today), source, \
                                                   action, \
                                                   product, \
                                                   interface, \
                                                   device, \
                                                   type);
#endif
/* if we want some logging of hotplug events we write the infos to /tmp/hotplug.event */
    FILE *hotplug_log = fopen("/tmp/hotplug.event", "a+");
    fprintf(hotplug_log, "\n\n   Output enviroments given by the kernel ...\n");
    fprintf(hotplug_log, "\n##HOTPLUG EVENT##\n"
                             "           date: %s"
                             "         source: %s\n"
                             "         action: %s\n"
                             "        product: %s\n"
                             "      interface: %s\n"
                             "         device: %s\n"
                             "class/subclass/protokoll: %s\n\n", ctime(&today), source, 
                                                               action, 
                                                               product, 
                                                               interface, 
                                                               device, 
                                                               type);
    fclose(hotplug_log);

#ifdef DEBUG
	fclose(debug);
#endif
}

void debug_info(char *infotext){
    FILE *debug = fopen("/dev/console", "w");
    fprintf(debug,"%s", infotext);
    fclose(debug);
}

void out_info(char *infotext) {
    FILE *info = fopen("/dev/console", "w");
    fprintf(info,"%s", infotext);
    fclose(info);
}
