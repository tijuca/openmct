/* owi2.h
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 * this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 */

#ifndef OWI2_H_
#define OWI2_H_
#endif /*OWI2_H_*/
#ifdef DEBUG
char debug_buf[600];   // for debugging output
#endif

#define MAX_PAIRS 255
#define HARDDISKMODEL	"/proc/ide/ide0/hda/model"
#define HARDDISK	"/dev/ide/host0/bus0/target0/lun0/disc"

struct CGI_DATA
{
    char *variable;
    char *value;
    struct CGI_DATA *next;
};

// structure for holding the status of services to boot or not
struct s_param
{
    char *telnetd;
    char *sshd;
    char *portmap;
    char *nfs;
    char *leds;
    char *ftpd;
    char *smartd;
    char *swap;
    char *syslogd;
    char *vservers;
    char *hdparm;
    char *crond;
    char *samba;
    char *ntpdate;
    char *httpd;
    char *ethtool_eth0;
    char *ethtool_eth1;
} rc_start;

// structure for the options of some services
struct s_opt
{
    /* heartbeat led */
    char *hb;                   // function for the HB LED
    int   blinkon;		// ontime if HB LED is set to 'blink'
    int   blinkoff;		// offtime if HB LED is set to 'blink'
    /* LED row 'IN' */
    char *led_in_device;        // Interface for LED row 'IN'
    char *led_in_direction;	// traffic direction for LED row 'IN'
    int   led_in_interval;	// Maxium value (in kB) for LED row 'IN'
    /* LED row 'OUT' */
    char *led_out_device;       // Interface for LED row 'OUT'
    char *led_out_direction;    // traffic direction for LED row 'OUT'
    int   led_out_interval;     // Maxium value (in kB) for LED row 'OUT'
    /* ethtool for eth0 */
    char *ethtool_eth0_dm;      // Duplexmodus eth0
    char *ethtool_eth0_nego;    // Autonegotation on or off
    int   ethtool_eth0_speed;   // networkspeed on eth0
    char *ethtool_eth0_wol;     // WakeOnLan ?
    /* ethtool for eth1 */
    char *ethtool_eth1_dm;      // Duplexmodus eth1
    char *ethtool_eth1_nego;    // Autonegotation on or off
    int   ethtool_eth1_speed;   // networkspeed on eth1
    char *ethtool_eth1_wol;     // WakeOnLan ?
    /* hint for Wake On Lan !
     * the realtek cards supports varius modi
     * look with 'ethtool eth[0,1]
     * 'Supports Wake-on: pumbg' <-- this are the supported modi
     * 'Wake-on: bg' <-- this are the setted modi
     * the modi can be:  p   Wake on physical activiti
     *                   u   Wake on Unicast-message
     *                   m   Wake on Multicast-message
     *                   b   Wake on Broadcast-message
     *                   a   Wake on ARP request
     *                   g   Wake on MagicPacket(tm)
     *                   s   activate SecureOn(tm) Passwort for MagicPacket(tm)
     *                   d   deactivated (wake on notthing). These option delete all other options! */
    /* NTP date server ip */
    char *ntpip;
    /* IP Forwarding */
    char *ip_forwarding;
    /* harddisk */
    char *hd_device;           // devicepath of the data partition
    int   hd_transfer_mode;    // UDMA modus, can the handle more the UDMA2 = 66 ??
    int   hd_dma;              // set dma flag or not
    int   hd_interrupt_umask;  // set umaskirq or not
    int   hd_sector_count;     // sectors to read in one go
    int   hd_32bit;            // modus of multi IO support
    int   hd_spindown;         // spindowntime for hd
} options;

/* some variables that used in varios menus or info pages */
struct sysinfo
{
    // the hostname of the NAS
    char *hostname;
    // software revision
    char *rev;
    // vendor + model of the NAS
    char *vendor;
    // uptime
    char *uptime;
    // the kernelversion
    char *kernel;
    // CPU
    char *cpu;
    /*int uptime_year;		// Years Uptime
    int uptime_day;		// Days UpTime
    int uptime_hour;		// Hours UpTime
    int uptime_min;		// Minutes UpTime*/
    // absolut processes (first read)
    int total_procs1;
    // idle processes (first read)
    int idle_procs1;
} statistics;

struct hdinfo
{
    // the hd manufactor and typ
    char *modell;
    // the firmware
    char *fw;
    // the serial
    char *serial;
    // the buffersize (Cache)
    int cache;
    // the maximal possible sector count
    int maxsc;
    // the setted maximal sector count
    int maxscset;
    // transfermode (Pio, DMA or UDMA modus)
    char *tfmode;
    // hd cyclinder, heads and sectors
    int cyclinder;
    int heads;
    int sectors;
    // size of hd, netto is size with basis 1024, brutto is basis with 1000
    int size_netto;
    int size_brutto;
    // numbers of partitions on the hd
    int partition;
    // the size of the partion (in GB, if the fs_id is 83 the size is in MB because this is a swap partition)
    int size1;
    // the filesystem ID
    int fs_id1;
    // mountpoint of the partition
    char *mountpoint1;
    // the filesystem name
    char *fs1;
    // usage of the partition
    char *usage1;

    // the size of the partion (in GB, if the fs_id is 83 the size is in MB because this is a swap partition)
    int size2;
    // the filesystem ID
    int fs_id2;
    // mountpoint of the partition
    char *mountpoint2;
    // the filesystem name
    char *fs2;
    // usage of the partition
    char *usage2;
} hd;

/* struct for holding the infos for one single partition of the hd */
//struct hdpartitions {

//};

struct networking
{
    // interface eth0, eth1:0, ...
    char *iface;
    // MAC adress
    char * mac;
    // ip adress
    char *ip;
    // broadcast adress
    char *bcast;
};

/* debug output to the console
 * function: give output to the text console
 */
void debug_info(char *infotext);

char *Strdup(const char *);
//void print_location(char *);

int get_sysinfos(void);
// return 0 if everything is o.k.

int read_rcconf(void);      // read data from /var/etc/rc.conf
int write_rcconf(struct CGI_DATA *data);     // write data to /var/etc/rc.conf
unsigned char *read_cgi_entry(struct CGI_DATA *data, char *variable);
char *getdata();
void hex2ascii(char *);
char convert(char *);
void printf_error(char *);
struct CGI_DATA *create_cgi_list(char *);
void delete(struct CGI_DATA *daten);

void start_page(char *title); // the start/overview page
void glob_set(char *title); // the global settings page

// sysinfo.c
unsigned char *read_hd_model (void);
int get_hd_info(void);
int get_networking_info (void);
