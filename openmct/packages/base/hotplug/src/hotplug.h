/* hotplug.h
Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)

this file is part of the hotplug handler for OpenMCT (http://www.openmct.org)
*/

/* some definitions */
#define USB_CONFIG_FILE "/var/etc/usb.conf"
#define BACKUP 		"/tmp/usb.conf.backup"
#define NEW 		"/tmp/usb.conf.new"

#define OUT_INFO	out_info
#define OUT_WARN	(void out_warn(char *infotext))
#define OUT_DEBUG	(void debug_info(char *infotext))


// struct for usb devices
struct device {
/* given by getenv("PRODUCT") */
    int vendorid;			/* vendor ID */
    int productid;			/* product ID */
    int protoid;			/* protocol ID */

/* given by getenv("INTERFACE") */
    int classid;                /* class ID */
    int subclassid;	            /* subclass ID */
    int subclassprotoid;        /* subclassproto ID */

/* given by getenv("DEVICE") */
    int bus;                    /* SCSI bus number */
    int number;                 /* device number on the bus */

    char vendorname[20];        /* real vendorname */
    char vendortype[20];        /* real vendorname */
    char serial[25];            /* serial number of the device */
};

struct config {
    char typ[4];                /* type of config entry, like S: (Stick), C:(Camera), or P: (Printer) */
    int level;                  /* number of some level (for Debug meanly) */
    char serial[25];            /* serial number */
    char mountpath[40];         /* the appendix for the mountpath */
};

struct device hotplug;
// functions in output.c
void write_debug_output(char *source, char *action, char *product, char *interface, char *device, char *type);
void close_all_files();
void debug_info(char *infotext);
void out_info(char *infotext);

// functions in functions.c
/* read out the serial number from the requested USB device from /proc/bus/usb/devices */
int read_serial(FILE *usbdevice, int g_bus, int g_dev);
/* build the devicepath from readed info from /proc/scsi/usb-storage-[x]/[x] */
unsigned char *create_devicepath(char *vendorname, char *vendortype, char *serial);
/* some strings have spaces at the end, for better reading we remove this */
unsigned char *remove_spaces_end(char *input);
/* for some cases we can't use strings with spaces inside, we replace this with underlines */
unsigned char *replace_spaces(char *vendorname, char *vendortype);

// functions in config.c
/* parse the config file for given string serial returns the mountpath */
unsigned char *read_config(char *serial);
/* read the entry numbers in the configfile */
int read_config_entrys(void);
/* split a single given line, return the value after the ":" */
unsigned char *get_config_entry(char *buffer);
/* write a new config and append new entry */
void write_append2config(char *typ, char *serial, char *path, int vid, int pid);
/* write the default header entrys to the new USB_CONFIG_FILE */
void write_config_file_header (FILE *file);

