/* functions.c
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
#define PRE_MOUNTPATH "/dev/scsi/host%d/bus0/target0/lun0/part1"
#define USB_STORAGE  "/proc/scsi/usb-storage-"

/* read out the serial number from the requested USB device from /proc/bus/usb/devices */
int read_serial(FILE *usbdevice, int g_bus, int g_dev) {

    char buf[150];            // buffer for readed line from file pointer 
    char debug_buf[150];      // buffer for debuging output
    char tmp[150];            // temp field for sscanf
    char *serial=NULL,
     *vendorname=NULL,
     *vendortype=NULL;        // char pointers for the splitted infos
    char *zdebug_buf;

    int found=0;               // step parameter
    int r_bus=0;               // number of the readed bus
    int r_dev=0;               // number of the readed device

    snprintf(debug_buf,99,"[%s/%d] started, given parameters (bus=%d , dev=%d)\n",__FUNCTION__,__LINE__,g_bus,g_dev);
    debug_info(debug_buf);

    while ((fgets (buf, sizeof (buf), usbdevice) != NULL) && (found <4)){
        if(found <= 4){
            if(buf[0] == 'T'){
                if(sscanf (buf,"%s Bus=%d %s %s %s %s Dev#= %d  %s %s",tmp,&r_bus,tmp,tmp,tmp,tmp,&r_dev,tmp,tmp)== 9){
                    if (g_bus == r_bus && g_dev == r_dev){
                        found = 1;
#ifdef debug
                        snprintf(debug_buf,99,"[%s/%d] given parameters (bus=%d , dev=%d)\n",__FUNCTION__,
                                                                                             __LINE__,
                                                                                             g_bus,
                                                                                             g_dev);
                        debug_info(debug_buf);
#endif //debug
                    }
                } else { /* give up and return some default values */
                    snprintf(debug_buf,99,"[%s/%d] could't read bus+device!\n",__FUNCTION__,__LINE__);
                    debug_info(debug_buf);
                    strcpy(hotplug.vendorname,"not found");
                    strcpy(hotplug.vendortype,"not found");
                    strcpy(hotplug.serial,"not found");
                    return -1;
                }
            }
            // proofing for ' buf[0] ...' is really needed! We may have a outcommented line!
            if (buf[0] == 'S' && found == 1 && 
                (strstr(buf,"S:  Manufacturer="))) {
                zdebug_buf=strtok(buf,"=");    // cutting all from the string before the "="
                vendorname=strtok(NULL,"\n");  // cutting the new line at the end of the string
                found=2;
                strcpy(hotplug.vendorname, remove_spaces_end(vendorname));
            }
            // proofing for ' buf[0] ...' is not really needed!
            if (buf[0] == 'S' && found == 2 &&
                (strstr(buf,"S:  Product="))) {
                zdebug_buf=strtok(buf,"=");		/* cutting all from the string before the "=" */
                vendortype=strtok(NULL,"\n");	/* cutting the new line at the end of the string */
                strcpy(hotplug.vendortype, remove_spaces_end(vendortype));
                found=3;
            }
            // proofing for ' buf[0] ...' is not really needed!
            if	(found == 3 && (strstr(buf,"S:  SerialNumber="))) {
                zdebug_buf=strtok(buf,"=");
                serial=strtok(NULL,"\n");
                strcpy(hotplug.serial,serial);
                found = 4;
            }
        }
    }
    return found;
}

/* build the devicepath from readed info from /proc/scsi/usb-storage-[x]/[x] */
unsigned char *create_devicepath(char *vendorname, char *vendortype, char *serial){

    FILE *storage=NULL;
    unsigned static char mountpath[80]; // return value of the function
    unsigned char openfile[100];        // dreated filename for fopen
    unsigned char append[10];           // path appendix and file for Filepointer
    char buf[50], *tmp=NULL;            // buffer for readed line from file pointer
    char g_usb_vendor[25],              // readed USB Vendor  from /proc/scsi/usb-storage-x/x
         g_usb_product[20],             // readed USB Product from /proc/scsi/usb-storage-x/x
         g_usb_serial[20],              // readed USB Serial  from /proc/scsi/usb-storage-x/x
     g_usb_attached[4];                 // attached Yes/No

    int g_usb_host=-1;                  // readed hostnumber for later to created the mountpath
    int path, file, ok=6;               // variables for getting the file to open

    char debug_buf[150];                // buffer for debuging output

    /* generating the path appendix and the filename */
    for (path=0,file=0 ;ok>1 && path <=10; path++,file++){

        snprintf(debug_buf,149,"[%s/%d] for() processed - path/file = %d/%d    (o.k. = %d)\n",__FUNCTION__,__LINE__,path,file,ok);
        debug_info(debug_buf);
        strcpy(openfile, USB_STORAGE);
        sprintf(append, "%d/%d",path,file);
        strcat(openfile,append);
        if ((storage=fopen(openfile, "r")) !=NULL){
            ok=5;
            snprintf(debug_buf,149,"[%s/%d] open %s o.k. (o.k. = %d)\n",__FUNCTION__,__LINE__,openfile,ok);
            debug_info(debug_buf);
            while ((fgets (buf, sizeof (buf), storage) != NULL) && ok<=5){
                if (strstr(buf,"Host")){
                    sscanf(buf, "   Host scsi%d: usb-storage",&g_usb_host);
                    snprintf(debug_buf,149,"[%s/%d] readed hostnumber: %d\n",__FUNCTION__,__LINE__,g_usb_host);
                    debug_info(debug_buf);
                }
                if (strstr(buf,"Vendor")){
                    tmp=strtok(buf," ");
                    tmp=strtok(NULL,"\n");
                    strcpy(g_usb_vendor,remove_spaces_end(tmp));
                    snprintf(debug_buf,99,"[%s/%d] vendor given/cleaned \"%s\"/\"%s\"\n",__FUNCTION__,__LINE__,tmp,g_usb_vendor);
                    debug_info(debug_buf);
                    if (strcmp(vendorname,g_usb_vendor) == 0){
                        ok=4;
                        snprintf(debug_buf,149,"[%s/%d] vendor compare status:     true      (o.k. = %d)\n",__FUNCTION__,__LINE__,ok);
                        debug_info(debug_buf);
                    }
                    tmp=NULL;
                }
                if (strstr(buf,"Product")&& (ok==4)){
                    tmp=strtok(buf," ");
                    tmp=strtok(NULL,"\n");
                    strcpy(g_usb_product,remove_spaces_end(tmp));
                    snprintf(debug_buf,149,"[%s/%d] product given/cleaned \"%s\"/\"%s\" \n",__FUNCTION__,__LINE__,tmp,g_usb_product);
                    debug_info(debug_buf);
                    if (strcmp(vendortype,g_usb_product) == 0){
                        ok=3;
                        snprintf(debug_buf,149,"[%s/%d] product compare status:    true      (o.k. = %d)\n",__FUNCTION__,__LINE__,ok);
                        debug_info(debug_buf);
                    }
                    tmp=NULL;
                }
                if (strstr(buf,"Serial")&& (ok==3)){
                    tmp=strtok(buf," ");
                    tmp=strtok(NULL," ");
                    tmp=strtok(NULL,"\n");
                    strcpy(g_usb_serial,tmp);
                    if (strcmp(serial,g_usb_serial) == 0){
                        ok=2;
                        snprintf(debug_buf,149,"[%s/%d] serial compare status:     true      (o.k. = %d)\n",__FUNCTION__,__LINE__,ok);
                        debug_info(debug_buf);
                    }
                    tmp=NULL;
                }
                if (strstr(buf,"Attached")&& (ok==2)){
                    ok=1;
                    tmp=strtok(buf,":");
                    tmp=strtok(NULL,"\n");
                    strcpy(g_usb_attached,tmp);
                    snprintf(debug_buf,149,"[%s/%d] attached status:          %s       (o.k. = %d)\n",__FUNCTION__,__LINE__,g_usb_attached,ok);
                    debug_info(debug_buf);
                    tmp=NULL;
                }
            }
        } else {
            debug_info("open failed\n");
        }
        if (storage != NULL)
            fclose(storage);
    }
    /* now we have all needed infos to build the mountpath 
    we use the PRE_MOUNTPATH macro to complet the path
    */
    snprintf(mountpath,sizeof(mountpath)-1,PRE_MOUNTPATH,g_usb_host);
    //debug_info(mountpath);
    return mountpath;
}

/* some strings have spaces at the end, for better reading we remove this */
unsigned char *remove_spaces_end(char *input){
	
    //char debug_buf[150];			// buffer for debuging output
    char help[20]="\0";
    unsigned static char output[30];
    int r_length=0, z=0;

    r_length=strlen(input);
    //snprintf(debug_buf,99,"[%s/%d] string length = %d input = \"%s\"\n",__FUNCTION__,__LINE__,r_length,input);
    //debug_info(debug_buf);
    if ((input[r_length-1]<=0x21) ){
        int last=1, spaces=0;
        for (z=r_length-1;  z>= 0; z--){
            if ((input[z]<=0x21) && (last==1)){
                spaces++;
                last=1;
            } else
                last=0;
        }
        strncpy(help,input,r_length-spaces);
        strcpy(output,help);
        //snprintf(debug_buf,99,"[%s/%d] string given/cleaned \"%s\"/\"%s\" \n",__FUNCTION__,__LINE__,input,output);
        //debug_info(debug_buf);
        input=NULL;
        return output;
    } else {
        strcpy(output,input);
        input=NULL;
        return output;
    }
}

/* for some cases we can't use strings with spaces inside, we replace this with underlines */
unsigned char *replace_spaces(char *vendorname, char *vendortype) {
	
    char *ret=NULL;
    int l=0, l_vn=0, l_vt=0;
	int loop;

    if(vendorname) {
        if (strlen(vendorname) > 0) {
            ret=strdup(vendorname);
        }
    }
    if (vendortype) {
        strcat(ret,"_");
        l_vt = strlen(vendortype);
        if (l_vt > 0) {
            if (ret) {
                l_vn = strlen(ret);
                ret = realloc(ret, l_vn + l_vt + 1);
                strcpy(ret + l_vn, vendortype);
            } else 
                ret = strdup(vendortype);
        }
    }
    l=strlen(ret);
    if (ret) {
       for (loop=0;loop<=l_vn+l_vt;loop++){
            if (ret[loop] == ' ')
                ret[loop] = '_';
       }
    }
	return ret;
}
