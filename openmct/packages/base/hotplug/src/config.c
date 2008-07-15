/* config.c
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)

this file is part of the hotplug handler for OpenMCT (http://www.openmct.org)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hotplug.h"

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

/* create a new config file */
int create_config_file() {
    FILE *new;
    
    // writing the new file
    if ((new = fopen(NEW_CONFIG_FILE, "w")) == NULL)
        return -1;
    // first write the header
    write_config_file_header(new);
    return 0;
}

/* parse the config file with given string serial and returns the mountpath */
unsigned char *read_mountpath(char *serial) {

    FILE *config;
    int  d_level=0,
              ii=0,
            loop=0;
    extern int configentrys;
    extern int have_config_file;
    char in[250];
    char r_serial[25]="\0";
// #info change to pointer!
    unsigned static char r_mountpath[25]="\0";  // returnvalue

    char debug_buf[150];				// buffer for debuging output

    if (have_config_file == -1)
        return "noconfigfile";
    else {
        snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] configfile entrys are: %d\n",__FUNCTION__,__LINE__
                                                                    ,configentrys);
        debug_info(debug_buf);
        if ((config = fopen(USB_CONFIG_FILE, "r")) != NULL) {
            while (fgets (in, sizeof (in), config) != NULL) {
                if (strstr(in,"D:") && (in[0]=='D')) {
                    sscanf(in,"D:%d",&d_level);
                    snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found debug level = \"%d\"\n",__FUNCTION__,__LINE__
                                                                                 ,d_level);
                    debug_info(debug_buf);
                }
                if ((strstr(in,"S:{") || strstr(in,"S{")) && ((in[0]=='s') || (in[0]=='S'))) {
                    while ((fgets (in, sizeof (in), config) && (strstr(in,"}")==NULL))) {
                        // remove the newline character from buffer if there
                        for (ii = 0; ii < sizeof(in); ii++)
                                if (in[ii] == '\n')
                                    in[ii] = '\0';
                        if (strstr(in,"serial")) {
                            strcpy(r_serial,get_config_entry(in));
                            snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] serial= \"%s\"\n",__FUNCTION__,__LINE__
                                                                             ,r_serial);
                            debug_info(debug_buf);
                        }
                        if (strstr(in,"mountpath")) {
                            strcpy(r_mountpath,get_config_entry(in));;
                            snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] mountpath= \"%s\"\n",__FUNCTION__,__LINE__
                                                                                ,r_mountpath);
                            debug_info(debug_buf);
                            if (strcmp(serial,r_serial)==0) { // if we found the serial give back the mountpath
                                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] MATCH found!\n",__FUNCTION__,__LINE__);
                                debug_info(debug_buf);
                                fclose(config);
                                return r_mountpath;
                            } else {
                                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] NO MATCH! try next ...\n",__FUNCTION__,__LINE__);
                                debug_info(debug_buf);
                            }
                        }
                    }
                } //else {
                    //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] no valid entry\n%d %s",__FUNCTION__,__LINE__,loop+1,in);
                    //debug_info(debug_buf);
                //}
                loop++;
            }
        } else {
            snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] could't open \"%s\"\n",__FUNCTION__,__LINE__
                                                                  ,USB_CONFIG_FILE);
            debug_info(debug_buf);
        }
    }
    return "error";
}

/* read the absolut numbers of entrys in the configfile
   if no configfile is availible returnvalue is -1 */
int read_config_entrys(void) {

    FILE *config;
    int entrys=0, read=0, loop=0;
    char in[250];
    char debug_buf[150];				// buffer for debuging output

    if ((config = fopen(USB_CONFIG_FILE, "r")) == NULL)
        return -1;
    else {
        while (fgets (in, sizeof (in), config) != NULL) {
            loop++;
            /* if we found in a privios run a starting tag for an entry "[x]{" put some information out */
            if ((strstr(in,"S:{") || strstr(in,"S{") || strstr(in,"C:{") || strstr(in,"C{")) && (read==1) && (in[0]!='#')) {
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found missplaced \"[x]{\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
                debug_info(debug_buf);
                read=2;
            }
            /* we search for a starting tag of an entry */
            if ((strstr(in,"S:{") || strstr(in,"S{") || strstr(in,"C:{") || strstr(in,"C{")) && (read==0) && (in[0]!='#')) {
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
                //debug_info(debug_buf);
                read=1;
            }
            /* if we have found in a privios run a starting tag for an entry "[x]{" we put NO infos out because this is done 
               in the "if loop" above! we only set the read variable back from "2" to "1"*/
            if ((strstr(in,"S:{") || strstr(in,"S{") || strstr(in,"C:{") || strstr(in,"C{")) && (read==2) && (in[0]!='#')) {
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
                //debug_info(debug_buf);
                read=1;
            }
            /* if we found in a privios run a closing tag for an entry "}" put some information out */
            if (strstr(in,"}") && (read==0) && (in[0]!='#')) {
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found missplaced \"}\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
                debug_info(debug_buf);
            }
            /* we search for a closing tag of an entry */
            if (strstr(in,"}") && (read==1) && (in[0]!='#')) {
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found= }\n",__FUNCTION__,__LINE__);
                //debug_info(debug_buf);
                entrys++;
                read = 0;
            }
        }
    }
    fclose(config);
    return entrys;
}

/* split a single given line, return the value after the ":" 
   everything after the string is ignored */
unsigned char *get_config_entry(char *buffer) {
	
    unsigned static char out[30];
    char *tmp=NULL,
      *second=NULL;
    char help[50]="\0";

    int length=0,
           pre=0,
         match=0,
         found=2,
       counter=0;
		
    length=strlen(buffer);
    while ((found>0) && (counter <= length) ) {
        counter++;
        if (buffer[counter] == ':') { /* found seperator */
            pre=counter;
            found=1;
        }
        if (((buffer[counter] == ' ') || (buffer[counter] == '\0')) && (found==1)) {
            match=counter;
            found=0;
        }
    }
    /* ugh, we have no ":" as seperator found? */
    if ((found>0) && (buffer[length] == '\0'))
        return "error";
    else {
    strncpy(help,buffer,match);
    tmp=strtok(help,":");
    second=strtok(NULL,"\0");
    strcpy(out,second);
    return out;
    }
}

/* this add a new entry to the usb.conf,
 * */
void write_append2config(char *typ, char *serial, char *path, int vid, int pid) {


    FILE *backup=NULL,
         *config=NULL,
         *new=NULL;
    char debug_buf[500];                // buffer for debuging output

    char line[250]="\0";
    char buf[200]="\0";
    char *puf=NULL;
    int loop=0, read=-1;
    //extern int configentrys;

    // first we make a backup copy
    if ((backup = fopen(BACKUP_CONFIG_FILE, "w")) == NULL)
        return;
    if ((config = fopen(USB_CONFIG_FILE, "r")) == NULL)
	   return;
    else {
        while (fgets (line, sizeof(line), config) != NULL) {
            fputs(line, backup);
        }
    }
    fclose(backup);
    fclose(config);
    // create the new file
    if ((new = fopen(NEW_CONFIG_FILE, "w")) == NULL)
        return;
    else {
        snprintf(buf,sizeof(buf)-1,"[%s/%d] open %s o.k.\n",__FUNCTION__,__LINE__,NEW_CONFIG_FILE);
        debug_info(buf);
    }
    // first write the header
    write_config_file_header(new);

    if ((config = fopen(USB_CONFIG_FILE, "r")) == NULL)
        return;
    else {
        snprintf(buf,sizeof(buf)-1,"[%s/%d] open %s o.k.\n",__FUNCTION__,__LINE__,USB_CONFIG_FILE);
        debug_info(buf);
        while (fgets (line, sizeof (line), config) != NULL) {
            loop++;

            // if we found in a previous run a starting tag for an entry "[x]{" free the space und release the pointer
            if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==1)) {
                // we found a curly brace that is missplaced, we do not copy this to the new file of cource
                read=-1;
                /*if puf {
                    free(puf);
                    puf=NULL;
                }*/
            }
            // we search for a starting tag of an entry
            if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==-1) /*&& (line[0]!='#')*/) {
                read=0;              
            }
            // if we have found in a previous run a starting tag for an entry "[x]{" we put NO infos out because this is done 
            // in the "if loop" above! we only set the read variable back from "2" to "1"
            if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==0)) {
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
                debug_info(debug_buf);
                //read=1;
            }
            // if we found in a previous run a closing tag for an entry "}" put some information out
            if (strstr(line,"}") && (read==2)) {
                // we found a curly brace that is missplaced, we do not copy this to the new file of cource
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found missplaced \"}\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
                debug_info(debug_buf);
                read=-1;
            }
            // we search for a closing tag of an entry
            if (strstr(line,"}") && (read==1) /*&& (line[0]!='#')*/) {
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found= }\n",__FUNCTION__,__LINE__);
                debug_info(debug_buf);
                //loop++;
                read = 2;
            }
            // filtering the demo entry from original file
            if (strstr(line,"serial:xxxxxxxxxxxxxxx")) {
                if (puf!=NULL) {
                    free(puf);
                    puf=NULL;
                    read=-1;
                }
            }
            // o.k. we doing now the concating of the whole config entry
            if (read == 1) { // read is only 1 if the have allready found the starting tag so just copy the line
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] old puf(%d)= \n\"%s\"\nto appended string lenght %d\n",__FUNCTION__,__LINE__,strlen(puf),puf,strlen(line));
                //debug_info(debug_buf);
                puf = realloc(puf,(strlen(puf) + strlen(line) +1) );
                strcat(puf,line);
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] new puf(%d)= \n\"%s\"\n",__FUNCTION__,__LINE__,strlen(puf),puf);
                //debug_info(debug_buf);
            }
            if (read == 0) { // read is 0 if have found a starting tag, line is now a part of an entry
                puf = realloc(NULL, strlen(line)+1);
                strcpy(puf,line);
                //snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] found string(%d):\n\"%s\"\n",__FUNCTION__,__LINE__,loop,puf);
                //debug_info(debug_buf);
                read=1;
            }
            if (read == 2) { // we found the closing tag
                puf = realloc(puf,(strlen(puf) + strlen(line)+1)); // realloc the size of *puf the size of the old length of *puf plus the length of the line
                strcat(puf,line);
                snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] concating string = \n\"%s\"\n",__FUNCTION__,__LINE__,puf);
                debug_info(debug_buf);
                fprintf(new,puf);
                read=-1;
                free(puf);
                puf=NULL;
            }
            //snprintf(debug_buf,sizeof(debug_buf)-1,"\n[%s/%d] current line(%d,%d): %s",__FUNCTION__,__LINE__,loop,read,line);
            //debug_info(debug_buf);
        }
        fclose(config);
        // add the new entry
        write_new_config_entry(new,typ,serial,path,vid,pid);
        fclose(new);
        // replace the new config
        system("mv /tmp/usb.conf.new /var/etc/usb.conf"); // system (foo) sucks! 
        // better use  int rename( const char *old, const char *new );
    }
}

/* this write the headerentry of the usb.conf */
void write_config_file_header (FILE *file) {
    fprintf(file,"# This is the configuration file for the hotplughandler of OMCT.\n");
    fprintf(file,"# Edit this file for your own needs.\n");
#ifdef DEBUG
    fprintf(file,"# If the hotplug binary is supporting debugging output you can set up\n"
                 "# the verbosity level.\n"
                 "# For debugging output you need a line with \"D:[x]\"\n"
                 "# possibly entry for [x] are: \"0\" - no output\n"
                 "#                             \"1\" - output of warnings to the console\n"
                 "#                             \"2\" - output of warnings to the console and to file\n");
    fprintf(file,"# uncomment this to enable debugging output\n"
                 "#D:1\n#\n");
#endif //DEBUG
    fprintf(file,"# Entrys must tagged by a specifying letter plus starting and a closing curly brace.\n"
                 "# The entrys have no spaces allowed!\n");
    fprintf(file,"# For example:\n#S{\n");
    fprintf(file,"#     serial:xxxxxxxxxxxxxxx     /* put some comment here if you want */\n");
    fprintf(file,"#  mountpath:whateveryouwant     /* put some comment here if you want */\n");
    fprintf(file,"# additional:my shiny stick      /* put some comment here if you want */\n");
    fprintf(file,"#}\n#\n# valid specifying letters are:\n# \"S\" USB-[S]ticks\n# \"C\" [C]amera (in the future)\n#\n");
    debug_info("writing file header done!\n");	
}

/* write a new entry to the new usb.conf */
void write_new_config_entry (FILE *file, char *typ, char *serial, char *path, int vid, int pid) {
    fprintf(file,"%s{\n",typ);
    fprintf(file,"     serial:%s /* put your comment here if you want */\n",serial);
    fprintf(file,"  mountpath:%s /* put your comment here if you want */\n",path);
    fprintf(file," additional:vid %x - pid %x  /* put your comment here if you want */\n", vid,pid);
    fprintf(file,"}\n");
}
