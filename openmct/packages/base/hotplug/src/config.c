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

unsigned char *read_config(char *serial) {
	FILE *config;
	int  d_level=0,
			  ii=0,
			loop=0;
	extern int configentrys;
	extern int have_config_file;
	char in[250];
	char r_serial[25]="\0";
	unsigned static char r_mountpath[25]="\0";
	
	char debug_buf[150];				// buffer for debuging output
	
	if (have_config_file == -1)
		return "noconfigfile";
	else {
		struct config usbconf[configentrys];
		snprintf(debug_buf,149,"[%s/%d] configfile entrys are: %d\n",__FUNCTION__,__LINE__,configentrys);
		debug_info(debug_buf);
		if ((config = fopen(USB_CONFIG_FILE, "r")) != NULL) {
			while (fgets (in, sizeof (in), config) != NULL) {
				if (strstr(in,"D:")) {
					sscanf(in,"D:%d",&d_level);
				strcpy(usbconf[loop].typ,"D:");
				usbconf[loop].level=d_level;
				}
				if ((strstr(in,"S:{") || strstr(in,"S{")) && (in[0]!='#')) {
					strcpy(usbconf[loop].typ,"S:");
					while ((fgets (in, sizeof (in), config) && (strstr(in,"}")==NULL))) {
						/* remove the newline character from buffer */
						for (ii = 0; ii < sizeof(in); ii++)
							if (in[ii] == '\n')
								in[ii] = '\0';
						if (strstr(in,"serial")) {
							strcpy(r_serial,get_config_entry(in));
							strcpy(usbconf[loop].serial,r_serial);
							snprintf(debug_buf,149,"[%s/%d] serial= \"%s\"\n",__FUNCTION__,__LINE__,r_serial);
							debug_info(debug_buf);
						}
						if (strstr(in,"mountpath")) {
							strcpy(r_mountpath,get_config_entry(in));
							strcpy(usbconf[loop].mountpath,r_mountpath);
							snprintf(debug_buf,149,"[%s/%d] mountpath= \"%s\"\n",__FUNCTION__,__LINE__,r_mountpath);
							debug_info(debug_buf);
							if (strcmp(serial,r_serial)==0) { /* if we found the serial give back the mountpath */
								snprintf(debug_buf,149,"[%s/%d] MATCH found!\n",__FUNCTION__,__LINE__);
								debug_info(debug_buf);
								fclose(config);
								return r_mountpath;
							} else {
								snprintf(debug_buf,149,"[%s/%d] NO MATCH! try next ...\n",__FUNCTION__,__LINE__);
								debug_info(debug_buf);
							}
						}
					}
				}
				loop++;
			}
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
				snprintf(debug_buf,149,"[%s/%d] found missplaced \"[x]{\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
				debug_info(debug_buf);
				read=2;
			}
			/* we search for a starting tag of an entry */
			if ((strstr(in,"S:{") || strstr(in,"S{") || strstr(in,"C:{") || strstr(in,"C{")) && (read==0) && (in[0]!='#')) {
				//snprintf(debug_buf,149,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				read=1;
			}
			/* if we have found in a privios run a starting tag for an entry "[x]{" we put NO infos out because this is done 
			   in the "if loop" above! we only set the read variable back from "2" to "1"*/
			if ((strstr(in,"S:{") || strstr(in,"S{") || strstr(in,"C:{") || strstr(in,"C{")) && (read==2) && (in[0]!='#')) {
				//snprintf(debug_buf,149,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				read=1;
			}
			/* if we found in a privios run a closing tag for an entry "}" put some information out */
			if (strstr(in,"}") && (read==0) && (in[0]!='#')) {
				snprintf(debug_buf,149,"[%s/%d] found missplaced \"}\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
				debug_info(debug_buf);
			}
			/* we search for a closing tag of an entry */
			if (strstr(in,"}") && (read==1) && (in[0]!='#')) {
				//snprintf(debug_buf,149,"[%s/%d] found= }\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				entrys++;
				read = 0;
			}
		}
	}
	fclose(config);
	return entrys;
}

unsigned char *get_config_entry(char *buffer) {
	
	unsigned static char out[30];
	char *tmp=NULL,*second=NULL;
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

void write_append2config(char *typ, char *serial, char *path, int vid, int pid) {
	
	FILE *backup=NULL, *config=NULL, *new=NULL;
	
	char line[250]="\0";
	char buf[200]="\0";
	
	int loop=0, read=0;
	//extern int configentrys;

	/* first we make a backup copy */
	if ((backup = fopen(BACKUP, "w")) == NULL)
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
	/* writing the new file */
	if ((new = fopen(NEW, "w")) == NULL)
		return;
	// first write the header
	write_config_file_header(new);
	if ((config = fopen(USB_CONFIG_FILE, "r")) == NULL)
		return;
	else {
		snprintf(buf,sizeof(buf)-1,"[%s/%d] open %s o.k.\n",__FUNCTION__,__LINE__,USB_CONFIG_FILE);
		debug_info(buf);
		while (fgets (line, sizeof (line), config) != NULL) {
			loop++;
			/* if we found in a privios run a starting tag for an entry "[x]{" put some information out */
			if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==1)) {
				// we found a curly brace that is missplaced, we do not copy this to the new file of cource
				read=2;
			}
			/* we search for a starting tag of an entry */
			if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==0) && (line[0]!='#')) {
				//snprintf(debug_buf,149,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				read=1;
			}
			/* if we have found in a privios run a starting tag for an entry "[x]{" we put NO infos out because this is done 
			   in the "if loop" above! we only set the read variable back from "2" to "1"*/
			if ((strstr(line,"S:{") || strstr(line,"S{") || strstr(line,"C:{") || strstr(line,"C{")) && (read==2)) {
				//snprintf(debug_buf,149,"[%s/%d] found= x{\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				read=1;
			}
			/* if we found in a privios run a closing tag for an entry "}" put some information out */
			if (strstr(line,"}") && (read==0)) {
				// we found a curly brace that is missplaced, we do not copy this to the new file of cource
				//snprintf(debug_buf,149,"[%s/%d] found missplaced \"}\" in line %d! check your config!\" !\n",__FUNCTION__,__LINE__,loop);
				//debug_info(debug_buf);
			}
			/* we search for a closing tag of an entry */
			if (strstr(line,"}") && (read==1) && (line[0]!='#')) {
				//snprintf(debug_buf,149,"[%s/%d] found= }\n",__FUNCTION__,__LINE__);
				//debug_info(debug_buf);
				loop++;
				read = 0;
			}
			if (read == 1 || read ==0)
				fputs(line, new);
		}
		fclose(config);
		// add the new entry
		fprintf(new,"%s{\n",typ);
		fprintf(new,"     serial:%s /* put your comment here if you want */\n",serial);
		fprintf(new,"  mountpath:%s /* put your comment here if you want */\n",path);
		fprintf(new," additional:vid %x - pid %x  /* put your comment here if you want */\n", vid,pid);
		fprintf(new,"}\n");
		fclose(new);
		// replace the new config
		system("mv /tmp/usb.conf.new /var/etc/usb.conf");
	}
}

void write_config_file_header (FILE *file) {
	
	fprintf(file,"# This is the configuration file for the hotplughandler of OMCT.\n");
	fprintf(file,"# Edit this file for your own needs.\n");
	fprintf(file,"# Entrys must tagged by a specifying letter plus starting and a closing curly brace.\n"
				"# The entrys have no spaces allowed!\n");
	fprintf(file,"# For example:\n#S{\n");
	fprintf(file,"#     serial:xxxxxxxxxxxxxxx     /* put some comment here if you want */\n");
	fprintf(file,"#  mountpath:whateveryouwant     /* put some comment here if you want */\n");
	fprintf(file,"# additional:my shiny stick      /* put some comment here if you want */\n");
	fprintf(file,"#}\n#\n# valid specifying letters are:\n# \"S\" USB-[S]ticks\n# \"C\" [C]amera (in the future)\n#\n");	
}
