#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#endif /*FUNCTIONS_H_*/

#define NEW_RC_CONFIG_FILE		"/tmp/rc.conf.new"
#define RC_CONFIG_FILE			"/var/etc/rc.conf"

// definations for the defautls in rc.conf (based on r612)
#define DEFAULT_START_CROND         "no"
#define DEFAULT_START_ETHTOOL_ETH0  "yes"
#define DEFAULT_START_ETHTOOL_ETH1  "no"
#define DEFAULT_START_FTPD          "yes"
#define DEFAULT_START_HDPARM        "yes"
#define DEFAULT_START_HTTPD         "yes"
#define DEFAULT_START_LEDS          "yes"
#define DEFAULT_START_NFS           "yes"
#define DEFAULT_START_NTPDATE       "yes"
#define DEFAULT_START_PORTMAP       "yes"
#define DEFAULT_START_SAMBA         "no"
#define DEFAULT_START_SMARTD        "no"
#define DEFAULT_START_SSHD          "yes"
#define DEFAULT_START_SWAP          "no"
#define DEFAULT_START_SYSLOGD       "yes"
#define DEFAULT_START_TELNETD       "yes"
#define DEFAULT_START_VSERVERS      "no"

#define DEFAULT_OPTIONS_HB						"cpu"
#define DEFAULT_OPTIONS_LEDS_BLINK_ON			"100"
#define DEFAULT_OPTIONS_LEDS_BLINK_OFF			"100"
#define DEFAULT_OPTIONS_LEDS_IN_DEVICE			"eth0"
#define DEFAULT_OPTIONS_LEDS_IN_DIRECTION		"r"
#define DEFAULT_OPTIONS_LEDS_IN_INTERVAL		"100"
#define DEFAULT_OPTIONS_LEDS_OUT_DEVICE			"eth0"
#define DEFAULT_OPTIONS_LEDS_OUT_DIRECTION		"t"
#define DEFAULT_OPTIONS_LEDS_OUT_INTERVAL		"100"

#define DEFAULT_OPTIONS_HDPARM_DEVICE           "/dev/ide/host0/bus0/target0/lun0/disc"
#define DEFAULT_OPTIONS_HDPARM_TRANSFER_MODE    "66"  /* UDMA modus, 66=UDMA2 */
#define DEFAULT_OPTIONS_HDPARM_DMA              "1"   /* use dma or not, 1=yes 0=no */
#define DEFAULT_OPTIONS_HDPARM_INTERRUPT_UNMASK "1"   /* use demasking of other interrupts, 1=yes 0=no */
#define DEFAULT_OPTIONS_HDPARM_SECTOR_COUNT     "16"  /* numbers of sectors to read in one go */
#define DEFAULT_OPTIONS_HDPARM_32BIT            "3"   /* multi IO 32bit modus */
#define DEFAULT_OPTIONS_HDPARM_SPINDOWN_TIMEOUT "60"  /* spindown time for hd, if value is between 1 to 240,
                                                       spindown time (in sec) = value x 5sec
                                                       if value is between 241 to 251,
                                                       spindown time (in min) = (value - 240) x 30min,
                                                       value = 252, signified timeout of 21min,
                                                       value = 253, sets a vendor-defined timeout,
                                                       value = 255 is interpreted as 21 minutes plus 15 seconds  */
#define DEFAULT_OPTIONS_NTPDATE_SERVER			"192.53.103.104"

#define DEFAULT_ENABLE_IPFORWARDING				"no"

#define DEFAULT_OPTIONS_ETHTOOL_ETH0_DUPLEX		"full"
#define DEFAULT_OPTIONS_ETHTOOL_ETH0_AUTONEG	"off"
#define DEFAULT_OPTIONS_ETHTOOL_ETH0_SPEED		"100"
#define DEFAULT_OPTIONS_ETHTOOL_ETH0_WAKEONLAN	"bg"
#define DEFAULT_OPTIONS_ETHTOOL_ETH1_DUPLEX		"full"
#define DEFAULT_OPTIONS_ETHTOOL_ETH1_AUTONEG	"off"
#define DEFAULT_OPTIONS_ETHTOOL_ETH1_SPEED		"100"
#define DEFAULT_OPTIONS_ETHTOOL_ETH1_WAKEONLAN	"bg"

unsigned char *read_hostname();
unsigned char *read_hardware(int request);
unsigned char *get_uptime();
unsigned char *get_loadavg();
unsigned char *get_cpuinfo();
unsigned char *kernelversion();
unsigned char *get_memory();
double average_load_since_previous(void);
void set_rc_defaults(void);

