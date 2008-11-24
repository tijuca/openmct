/* debug.c
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)

this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "owi2.h"

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

void debug_info(char *infotext){
#ifdef DEBUG
    FILE *debug = fopen("/dev/console", "w");
    fprintf(debug,"%s", infotext);
    fclose(debug);
#endif
}
