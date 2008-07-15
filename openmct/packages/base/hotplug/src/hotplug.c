/* hotplug.c

/sbin/hotplug kernel helper application for hotplugabled devices
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de) 
(with friendly permission and idea by MichaR)

this file is part of the hotplug handler for OpenMCT (http://www.openmct.org)
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "hotplug.h"

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#define USB_STICK_MP "/media/usbstick/"

int have_config_file=0;     // used in read_mountpath
int configentrys=0;         // used in read_mountpath

int main(int argc, char *argv[], char *envp[]) {

    FILE *devices=NULL;

    char debug_buf[500];   // for debugging output
    char *devicepath=NULL;
    char *mountpath=NULL;

    if ((!argv[1]) || (!getenv("ACTION"))){
        // some kids may play by calling all the executables, so give an notice and quit
        printf ("-------------------------------------------------------------\n");
        printf ("Linux Hotplug Kernel-helper (C) carsten v%s\n",HOTPLUG_VERSION);
        printf ("this binary is called by the kernel! You can't use it without\n"
                "the given parameters by the kernel!\n");
        printf ("-------------------------------------------------------------\n");
        return 0;
    }
    // try to read the config file
    configentrys=read_config_entrys();
    if (configentrys > 0) { // if we found more then 0 entrys we have a config file
        have_config_file=1;
    } else {
        if (create_config_file() == -1) // still to complete!
            return -1;
    }
    
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] %s %s started\n",__FUNCTION__,__LINE__,PACKAGE_NAME,HOTPLUG_VERSION);
    debug_info(debug_buf);
    write_debug_output(argv[1], getenv("ACTION"), getenv("PRODUCT"), getenv("INTERFACE"), getenv("DEVICE"), getenv("TYPE"));

    // so we have to start with the splitting of the informations */
    if (strncmp(argv[1], "usb", 3) == 0){
        if (strncmp(getenv("ACTION"), "add", 3) == 0){
            // 1st we get the VendorID and ProductID
            if (sscanf(getenv("PRODUCT"), "%x/%x/%x", &hotplug.vendorid, &hotplug.productid, &hotplug.protoid) !=3 ){
                debug_info("could't get PRODUCT infos\n");
            }
            // 2nd we grab the ClassID and the SubClassID
            if (sscanf(getenv("INTERFACE"), "%d/%d/%d", &hotplug.classid, &hotplug.subclassid, &hotplug.subclassprotoid) != 3){
                debug_info("could't get INTERFACE infos\n");
            }
            // 3rd we grab the SCSI bus id and the product id
            if (sscanf(getenv("DEVICE"), "/proc/bus/usb/%d/%d",&hotplug.bus, &hotplug.number) != 2){
                debug_info("could't get DEVICE infos\n");
                return -1;
            }
            sleep(1); // if the kernel is just booted the procFS entry are'nt all there so we wait a little bit
            // 4rd we get the serial number of the device if it is usb-storage
            if (hotplug.classid == 8) {
                devices=fopen("/proc/bus/usb/devices", "r");
                if (devices != NULL){
                    int read = read_serial(devices,hotplug.bus,hotplug.number);
                    if (read != 4) {
                        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] read_serial failed! status %d\n",__FUNCTION__,__LINE__,read);
                        debug_info(debug_buf);
                        return -1; // nothing more can be done, exiting!
                    }
                }
            }
            // now we scan for removable storage (classid == 8)
            // in case of hotplug.class == 8 we have to make some action
            if (hotplug.classid == 8) {
                // we have now to search for the host number where the usb-storage
                // is bound to the file tree (we found this in /proc/scsi/usb-storage-x/x
                devicepath=create_devicepath(hotplug.vendorname, hotplug.vendortype, hotplug.serial);
                snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] devicepath for %s/%s = \n\t\"%s\"\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype, devicepath);
                debug_info(debug_buf);
                
                mountpath = read_mountpath(hotplug.serial);
                if (strcmp(mountpath,"noconfigfile")==0) {
                    // add some code here
                    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] no config file found! give up!\n",__FUNCTION__,__LINE__);
                    debug_info(debug_buf);
                    return -1;
                }
                if (strcmp(mountpath,"error")==0) { // if no config entry for the stick is found
                    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] no config entry found! generate new mountpath!\n",__FUNCTION__,__LINE__);
                    debug_info(debug_buf);
                    mountpath=replace_spaces(hotplug.vendorname,hotplug.vendortype);
                    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] generated mountpath= \"%s\"\n",__FUNCTION__,__LINE__,mountpath);
                    debug_info(debug_buf);
                    strcpy(debug_buf,"\0");
                    write_append2config("S",hotplug.serial, mountpath, hotplug.vendorid, hotplug.productid);
                }
                snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] mountpath for %s/%s = \n\t\""USB_STICK_MP"%s\"\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype, mountpath);
                debug_info(debug_buf);
                snprintf(debug_buf, sizeof(debug_buf)-1, "mkdir -p "USB_STICK_MP"%s\n", mountpath);
                system(debug_buf);
                snprintf(debug_buf, sizeof(debug_buf)-1, "mount %s "USB_STICK_MP"%s\n",devicepath, mountpath);
                
                if (mountpath) {
                    free(mountpath);
                    mountpath=NULL;
                }
                if (system(debug_buf)==0) { // change to != 0 for testing updating the usb.conf
                    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] %s/%s sucessfully mounted\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype);
                    debug_info(debug_buf);
                } else {
                    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] %s/%s mount failed!\n",__FUNCTION__,__LINE__,hotplug.vendorname, hotplug.vendortype);
                    debug_info(debug_buf);
                }
                strcpy(debug_buf,"\0");
            }

#ifdef DEBUG
            debug_info("   summary of infos getting by the hotplug binary\n");
            snprintf(debug_buf, sizeof(debug_buf)-1, "\nfound hotplug.vendorid   = %x\n"
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
            debug_info(debug_buf);
#endif
        }
    }
    fclose(devices);
    debug_info("\n-------------------------------\n");
    return 0;
}
