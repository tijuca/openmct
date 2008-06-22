/* hotplug.c

/sbin/hotplug kernel helper application for hotplugabled devices
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de) 
(with friendly permission by MichaR)

this file is part of the hotplug handler for OpenMCT (http://www.openmct.org)
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "hotplug.h"

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#define USB_STICK_MP "/media/usbstick/"

int have_config_file=0;		// used in read_config_entrys
int configentrys=0;			// used in read_config_entrys

int main(int argc, char *argv[], char *envp[]) {

	FILE *devices=NULL;
	
	char tmpinfo[1000];
	char *devicepath=NULL;
	char *mountpath=NULL;

    if ((!argv[1]) || (!getenv("ACTION"))){
	/* some kids may play by calling all the executables, so give an notice a quit */
	printf ("-------------------------------------------------------------\n");
	printf ("Linux Hotplug Kernel-helper (C) carsten %s\n",PACKAGE_VERSION);
	printf ("this binary is called by the kernel! You can't use it without\n"
		    "the given parameters by the kernel!\n");
	printf ("-------------------------------------------------------------\n");
	return 0;
    }
	snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] %s %s started\n",__FUNCTION__,__LINE__,PACKAGE_NAME,HOTPLUG_VERSION);
	OUT_INFO(tmpinfo);
	write_debug_output(argv[1], getenv("ACTION"), getenv("PRODUCT"), getenv("INTERFACE"), getenv("DEVICE"), getenv("TYPE"));

/* so we have to start with the splitting of the informations */
	if (strncmp(argv[1], "usb", 3) == 0){
		if (strncmp(getenv("ACTION"), "add", 3) == 0){
			/* try to read the config file */
			configentrys=read_config_entrys();
			if (configentrys > 0) // if we found more then 0 entrys we have a config file
				have_config_file=1;
			/* 1st we get the VendorID and ProductID */
			if (sscanf(getenv("PRODUCT"), "%x/%x/%x", &hotplug.vendorid, &hotplug.productid, &hotplug.protoid) !=3 ){
				debug_info("could't get PRODUCT infos\n");
				}
			/* 2nd we grab the ClassID and the SubClassID */
			if (sscanf(getenv("INTERFACE"), "%d/%d/%d", &hotplug.classid, &hotplug.subclassid, &hotplug.subclassprotoid) != 3){
				debug_info("could't get INTERFACE infos\n");
			}
			/* 3rd we grab the SCSI bus id and the product id */
			if (sscanf(getenv("DEVICE"), "/proc/bus/usb/%d/%d",&hotplug.bus, &hotplug.number) != 2){
				debug_info("could't get DEVICE infos\n");
			}
			sleep(1);
			/* 4rd we get the serial number of the device if it is usb-storage*/
			if (hotplug.classid == 8) {
				devices=fopen("/proc/bus/usb/devices", "r");
				if (devices != NULL){
					int read = read_serial(devices,hotplug.bus,hotplug.number);
					if (read != 4) {
						snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] read_serial failed! status %d\n",__FUNCTION__,__LINE__,read);
						OUT_INFO(tmpinfo);
						return 0; /* nothing more can be done, exiting! */
					}
				}
			}
			/* now we scan for removable storage (classid == 8)
			   in case of hotplug.class == 8 we have to make some action*/
			if (hotplug.classid == 8) {
				/* we have now to search for the host number where the usb-storage
				   is bound to the file tree (we found this in /proc/scsi/usb-storage-x/x */
				devicepath=create_devicepath(hotplug.vendorname, hotplug.vendortype, hotplug.serial);
				snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] devicepath for %s/%s = \n\t\"%s\"\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype, devicepath);
				debug_info(tmpinfo);
				mountpath = read_config(hotplug.serial);
				if (strcmp(mountpath,"noconfigfile")==0) {
					// add some code here
					snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] no config file found! give up!\n",__FUNCTION__,__LINE__);
					debug_info(tmpinfo);
					return -1;
				}
				if (strcmp(mountpath,"error")==0) { // if no config entry for the stick is found
					snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] no config entry found! generate new mountpath!\n",__FUNCTION__,__LINE__);
					debug_info(tmpinfo);
					mountpath=replace_spaces(hotplug.vendorname,hotplug.vendortype);
					snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] generated mountpath= \"%s\"\n",__FUNCTION__,__LINE__,mountpath);
					debug_info(tmpinfo);
					strcpy(tmpinfo,"\0");
					write_append2config("S",hotplug.serial, mountpath, hotplug.vendorid, hotplug.productid);
				}
				snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] mountpath for %s/%s = \n\t\""USB_STICK_MP"%s\"\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype, mountpath);
				debug_info(tmpinfo);
				snprintf(tmpinfo, sizeof(tmpinfo)-1, "mkdir -p "USB_STICK_MP"%s\n", mountpath);
				system(tmpinfo);
				snprintf(tmpinfo, sizeof(tmpinfo)-1, "mount %s "USB_STICK_MP"%s\n",devicepath, mountpath);
				// umstellen für testing
				if (system(tmpinfo)==0) {
					snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] %s/%s sucessfully mounted\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype);
					debug_info(tmpinfo);
				} else {
					snprintf(tmpinfo, sizeof(tmpinfo)-1, "[%s/%d] %s/%s mount failed!\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype);
					debug_info(tmpinfo);
				}
				strcpy(tmpinfo,"\0");
				
			}

#ifdef DEBUG
			debug_info("   summary of infos getting by the hotplug binary\n");
			snprintf(tmpinfo, sizeof(tmpinfo)-1, "\nfound hotplug.vendorid   = %x\n"
												 "found hotplug.productid  = %x\n"
												 "found hotplug.class      = %d\n"
												 "found hotplug.subclassid = %d\n"
												 "found hotplug.bus        = %d\n"
												 "found hotplug.number     = %d\n"
												 "found hotplug.serial     = %s\n"
												 "found hotplug.vendorname = %s\n"
												 "found hotplug.vendortype = %s\n",
												 hotplug.vendorid,
												 hotplug.productid,
												 hotplug.classid,
												 hotplug.subclassid,
												 hotplug.bus,
												 hotplug.number,
												 hotplug.serial,
												 hotplug.vendorname,
												 hotplug.vendortype);
			debug_info(tmpinfo);
#endif
		
		}
	}
    close_all_files();
	fclose(devices);
	debug_info("\n-------------------------------\n");
	return 0;
}
