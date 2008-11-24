/* p_start.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 *
 * all the helper functions for the dynamic in the html sides
 */
  #ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "owi2.h"
#include "functions.h"

#define PROC_STAT "/proc/stat"
#define CPU_FMT "cpu %lu %lu %lu %lu"

//static double average_load_since_previous(void);

/* function: getting some systems informations
 * return: nothing, yust fill the struct statistics
 */
int get_sysinfos(void) {
	// getting the hostname
	statistics.hostname=Strdup(read_hostname());
	// the revision of the image
	statistics.rev=Strdup(read_hardware(1));
	// the vendor + model
	statistics.vendor=Strdup(read_hardware(2));
	// the uptime
	statistics.uptime=Strdup(get_uptime());
	// the kernel version
	statistics.kernel=Strdup(kernelversion());
	// the CPU version
	statistics.cpu=Strdup(get_cpuinfo());
	return 0;
}

/* function: read the hostname from /etc/hostname
 * return: pointer with the name
 * on error: default string
 * */
unsigned char *read_hostname() {
    FILE *hostname = NULL;
    char buf[50];
    char *ret=NULL;

    if ((hostname=fopen("/etc/hostname", "r")) != NULL) {
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/etc/hostname' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
        if (fgets (buf, sizeof(buf), hostname) == NULL) {
            fclose(hostname);
            return "???";
        }
        buf[strlen(buf)-1]='\0';
#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] readed hostname: '%s' \n",__FUNCTION__,__LINE__,buf);
    debug_info(debug_buf);
#endif
        ret=Strdup(buf);
        fclose(hostname);
        return ret;
    }
    else
        return "???";
}

/* function: read the hardware from /etc/openmct-release
 * return: pointer with the name
 * on error: default string
 * */
unsigned char *read_hardware(int request) {
    FILE *release = NULL;
    //char buffer[50];
    char *ret=NULL;
    char model[10];
    char rev[10];
    char t[10];
    char prod_code[10];


    if ((release=fopen("/etc/openmct-release", "r")) != NULL) {
#ifdef DEBUG
        extern char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/etc/openmct-release' o.k.\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
        fscanf( release, "OpenMCT %s 0x100 %s %s %s\n", model, t, prod_code, rev);
        fclose(release);
#ifdef DEBUG
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] model=%s prod_code=%s revisision=%s\n",__FUNCTION__,__LINE__,model,prod_code,rev);
            debug_info(debug_buf);
#endif
        if (request==1){                        // software rev is requested
            ret=Strdup(rev);
            return ret;
        } else if (request==2) {                // vendor and model is requested
            if ((strcmp(prod_code,"0x04"))||    // SA110
                (strcmp(prod_code,"0x05"))||    // NAS110
                (strcmp(prod_code,"0x1001"))||  // SA200
                (strcmp(prod_code,"0x2001"))) { // NAS200
                ret=Strdup("Claxan ");
            } else if (strcmp(prod_code,"0x06")) { // IOGear
                ret=Strdup("IOGear ");
            //else if (strcmp(prod_code,"0x??")) { // Tritton and Intellinet ??
            } else {
                ret=Strdup("unknown ");
            }
        }
        ret=realloc(ret,strlen(ret)+strlen(model)+1);
        strcat(ret,model);
        fclose(release);
        return ret;
        //}
    } else {
        return "no-data";
    }
}

#warning "get_loadavg() is deprecated! -> switch to struct statistics"
unsigned char *get_loadavg() {
    FILE * load;
    char *ret=NULL;
    char buf[6];
    float user=0, nice=0, system=0, idle=0;
    float sys=0, total=0;
    float percent=0;

    if ( (load = fopen("/proc/stat", "r")) == NULL )
        return "???";
    else {
#ifdef DEBUG
        extern char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/stat' o.k.\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
    }
    /* the entry in /proc/stat means following for example
     *       user nice system idle
     * 'cpu  203  0    924    467551'
     * total = user + nice + system + idle
     * 468678= 203  + 0    + 924    + 467551
     * cpu load in percent = (total - idle)*100 / total
     *                0,2% = (468678-467551)*100 / 468678
     */
    fscanf( load, "cpu %f %f %f %f\n",&user,&nice,&system,&idle);
    if ( fclose(load) != 0) {
#ifdef DEBUG
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] error closing '/proc/stat'\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
        return "???";
    } else {
        sys=user+nice+system;
        total=user+nice+system+idle;
        percent = (100 * (sys - 0)) / (total);
        snprintf(buf, sizeof(buf)-1, "%.2f",percent);
//        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] readed: %.0fu %.0fn %.0fs %.0fi %f%% Load\n",__FUNCTION__,__LINE__,user,nice,system,idle,percent);
//        debug_info(debug_buf);
        ret=Strdup(buf);
    }
    return ret;
}

/* make use of /proc/uptime and the fact that it stores total idle time */
unsigned char *get_uptime() {
    FILE * p_uptime;
    float uptime1=0, uptime2=0;
    char *ret=NULL, string[60];
    unsigned y=0, d=0, h=0, m=0;

    if (( p_uptime = fopen("/proc/uptime", "r")) == NULL )
        return "???";
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/uptime' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    if ( fscanf(p_uptime, "%f %f", &uptime1, &uptime2) != 2)
        return "???";

    d = uptime1 / 86400;
    h = (uptime1 - (d * 86400)) / 3600;
    m = (uptime1 - (d * 86400) - (h * 3600)) / 60;
    if ((h==0) && (d==0) && (y==0))
        snprintf(string, sizeof(string)-1, "%umin",m);
    else if ((d==0) && (y==0))
        snprintf(string, sizeof(string)-1, "%uh %umin",h,m);
    else if (y==0)
        snprintf(string, sizeof(string)-1, "%dT %uh %umin",d,h,m);
    else
        snprintf(string, sizeof(string)-1, "%dJ %dT %uh %umin",y,d,h,m);
    ret=Strdup(string);
    // clean things up
    if ( fclose(p_uptime) != 0) {
#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] error closing '/proc/uptime'!\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
		return "???";
    }
    return ret;
}

unsigned char *get_cpuinfo() {
    // this functions givs only an average over the whole time since last start!
    // this here is better
    // http://www.koders.com/c/fidA14AFC28AEACAFE6E6BDB02F0A8D7F2345C642DC.aspx?s=%2fproc%2fstat#L2
    FILE *cpuinfo;
    char vendor[10], model[8], string [30], tmp[10];
    float clock=0;
    unsigned char *ret=NULL;

    if ( ( cpuinfo = fopen("/proc/cpuinfo", "r")) == NULL )
        return "???";
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/cpuinfo' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    fscanf(cpuinfo, "system type             : %s %s\n", vendor, tmp);
    fscanf(cpuinfo, "processor               : %s\n", tmp);
    fscanf(cpuinfo, "cpu model               : %s %s  %s %s\n", model,tmp,tmp,tmp);
    fscanf(cpuinfo, "BogoMIPS                : %f\n", &clock);
#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] read: '%s' '%s' %.2fMHz\n",__FUNCTION__,__LINE__,vendor, model, clock);
    debug_info(debug_buf);
#endif
    fclose(cpuinfo);
    snprintf(string, sizeof(string)-1, "%s %s %.2fMhz",vendor, model, clock);
    ret=Strdup(string);
    return ret;
}

/*
double average_load_since_previous (void) {
    FILE * f ;
    static unsigned long lastIdle=0, lastBusy=0 ;
    unsigned long user, nice, system, idle, busy ;
    int n ;
    double ret ;

    if ((f = fopen (PROC_STAT, "r")) == NULL)
        return 0 ;
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/stat' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    n = fscanf (f, CPU_FMT, &user, &nice, &system, &idle) ;
    fclose (f) ;
    if (n != 4)
        return 0 ;

    busy = user + nice + system ;
    if (busy==lastBusy)
        busy--;
    ret = (double)(busy-lastBusy) / (busy-lastBusy + idle-lastIdle) ;
#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] lastBusy: %lu lastIdle: %lu load: %f%%\n"
                                                "busy: %lu\n"
                                                ,__FUNCTION__,__LINE__,lastBusy,lastIdle,ret,busy);
    debug_info(debug_buf);
#endif
    lastBusy = busy ;
    lastIdle = idle ;

    // NB: the first measure is misleading
    return ret ;
}
*/

unsigned char *kernelversion() {
    FILE *ver;
    char version[16], sub[3], month[6], string[40], tmp[10];
    int day=0, year=0;
    unsigned char *ret=NULL;

    if ( ( ver = fopen("/proc/version", "r")) == NULL )
        return "???";
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/version' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
//               Linux version 2.4.35.3-mct (carsten@C2D6600) (gcc version 3.3.6) #2 Sa 29. Mär 13:47:52 CET 2008
    fscanf(ver, "%s    %s      %s           %s                 %s  %s      %s     %s %s %d. %s   %s       %s  %d\n"
                ,tmp,  tmp,    version,     tmp,               tmp,tmp,   tmp,    sub,tmp,&day, month, tmp, tmp, &year);

#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] read: %s %s %d. %s %d\n",__FUNCTION__,__LINE__,version, sub, day, month, year);
    debug_info(debug_buf);
#endif
    fclose(ver);
    // disabled for now, because the 'ä' 'ü' 'ö' need some special work
    //snprintf(string, sizeof(string)-1, "%s %s %d. %s %d",version, sub, day, month, year);
    snprintf(string, sizeof(string)-1, "%s %s (%d)",version, sub, year);
    ret=Strdup(string);
    return ret;
}

unsigned char *get_memory() {
    FILE *mem;
    int tmp;
    float total, free;
    char string[40], t[60];
    unsigned char *ret=NULL;

    if ((mem = fopen("/proc/meminfo", "r")) == NULL )
        return "???";
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/proc/meminfo' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    fscanf(mem, "        total:    used:    free:  shared: buffers:  cached%s\n", t);
    fscanf(mem, "Mem:  %f %d %f %d %d %d\n",&total, &tmp, &free, &tmp, &tmp, &tmp);
    fclose(mem);
    snprintf(string, sizeof(string)-1, "%.0fMB/%.0fMB (%.0f%% frei)",total/(1024*1024), free/(1024*1024), (float)(free*100)/total);
    ret=Strdup(string);
    return ret;
}
// getting memory data  total used free shared buffers cached
//    fscanf(p_uptime, "Mem:  %d    %d    %d  %d     %d       %d", &total, &used)

/* function: read in the data values from /var/etc/rc.conf
 * return 0 if success
 */
int read_rcconf(void) {
    FILE *rcconf;       // filepointer for /var/etc/rc.conf
    char in[70];        // buffer for reading the rc.conf entrys
    char *tmp=NULL;
    int loop=0;         // counter for while

    if ((rcconf = fopen(RC_CONFIG_FILE, "r")) == NULL )
        return 0;
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] open '/var/etc/rc.conf' o.k.\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    while (fgets (in, sizeof (in), rcconf) != NULL) {
        loop++;
        tmp=strtok(in,"="); // splitting the string on '='
#warning "No check for valid readed rc.conf values implemented!"
        if (strcmp(tmp,"START_CROND")==0) {                         // if the left side of the splitted string is ...
            rc_start.crond=Strdup(strtok(NULL,"\0"));               // we store the right side of the string
            rc_start.crond[strlen(rc_start.crond)-1]='\0';          // and cut off the \n at the end
        }
        else if (strcmp(tmp,"START_ETHTOOL_ETH0")==0) {
            rc_start.ethtool_eth0=Strdup(strtok(NULL,"\0"));
            rc_start.ethtool_eth0[strlen(rc_start.ethtool_eth0)-1]='\0';
        }
        else if (strcmp(tmp,"START_ETHTOOL_ETH1")==0) {
            rc_start.ethtool_eth1=Strdup(strtok(NULL,"\0"));
            rc_start.ethtool_eth1[strlen(rc_start.ethtool_eth1)-1]='\0';
        }
        else if (strcmp(tmp,"START_FTPD")==0) {
            rc_start.ftpd=Strdup(strtok(NULL,"\0"));
            rc_start.ftpd[strlen(rc_start.ftpd)-1]='\0';
        }
        else if (strcmp(tmp,"START_HDPARM")==0) {
            rc_start.hdparm=Strdup(strtok(NULL,"\0"));
            rc_start.hdparm[strlen(rc_start.hdparm)-1]='\0';
        }
        else if (strcmp(tmp,"START_HTTPD")==0) {
            rc_start.httpd=Strdup(strtok(NULL,"\0"));
            rc_start.httpd[strlen(rc_start.httpd)-1]='\0';
        }
        else if (strcmp(tmp,"START_LEDS")==0) {
            rc_start.leds=Strdup(strtok(NULL,"\0"));
            rc_start.leds[strlen(rc_start.leds)-1]='\0';
        }
        else if (strcmp(tmp,"START_NFS")==0) {
            rc_start.nfs=Strdup(strtok(NULL,"\0"));
            rc_start.nfs[strlen(rc_start.nfs)-1]='\0';
        }
        else if (strcmp(tmp,"START_NTPDATE")==0) {
            rc_start.ntpdate=Strdup(strtok(NULL,"\0"));
            rc_start.ntpdate[strlen(rc_start.ntpdate)-1]='\0';
        }
        else if (strcmp(tmp,"START_PORTMAP")==0) {
            rc_start.portmap=Strdup(strtok(NULL,"\0"));
            rc_start.portmap[strlen(rc_start.portmap)-1]='\0';
        }
        else if (strcmp(tmp,"START_SAMBA")==0) {
            rc_start.samba=Strdup(strtok(NULL,"\0"));
            rc_start.samba[strlen(rc_start.samba)-1]='\0';
        }
        else if (strcmp(tmp,"START_SMARTD")==0) {
            rc_start.smartd=Strdup(strtok(NULL,"\0"));
            rc_start.smartd[strlen(rc_start.smartd)-1]='\0';
        }
        else if (strcmp(tmp,"START_SSHD")==0) {
            rc_start.sshd=Strdup(strtok(NULL,"\0"));
            rc_start.sshd[strlen(rc_start.sshd)-1]='\0';
        }
        else if (strcmp(tmp,"START_SWAP")==0) {
            rc_start.swap=Strdup(strtok(NULL,"\0"));
            rc_start.swap[strlen(rc_start.swap)-1]='\0';
        }
        else if (strcmp(tmp,"START_SYSLOGD")==0) {
            rc_start.syslogd=Strdup(strtok(NULL,"\0"));
            rc_start.syslogd[strlen(rc_start.syslogd)-1]='\0';
        }
        else if (strcmp(tmp,"START_TELNETD")==0) {
            rc_start.telnetd=Strdup(strtok(NULL,"\0"));
            rc_start.telnetd[strlen(rc_start.telnetd)-1]='\0';
        }
        else if (strcmp(tmp,"START_VSERVERS")==0) {
            rc_start.vservers=Strdup(strtok(NULL,"\0"));
            rc_start.vservers[strlen(rc_start.vservers)-1]='\0';
        }

        /* now reading the single options of some values, starting with led section
		 * THx to Udo who made the patch aviable for OpenMCT!
         * (especialy more info in http://forum.openmct.org/thread.php?threadid=146)
         *
         * the heartbeat section can handle varius things:
         * 1. the functions of the HB LED
         *  in '/proc/mct/LED/hb' is 'cpu' written:             the HB LED shows the hearbeat of cpu usage
         *  in '/proc/mct/LED/hb' is 'on' written:              the HB LED is permanetly on
         *  in '/proc/mct/LED/hb' is 'off' written:             the HB LED is permanetly off
         *  in '/proc/mct/LED/hb' is 'blink 100 100' written:   the HB LED blinks for 1s on and 1s off
         * !FixME! the blink option is not implemented in the OWI!
         */

        else if (strcmp(tmp,"OPTIONS_LEDS_HB")==0) {	// the heartbeat LED
            options.hb=Strdup(strtok(NULL,"\0"));
            options.hb[strlen(options.hb)-1]='\0';
		//	 if (strcmp(options.hb,"blink")==0) {			// if the heartbeat is set to 'blink' read the next value
		}
		else if (strcmp(tmp,"OPTIONS_LEDS_BLINK_ON")==0) {
			sscanf(strtok(NULL,"\0"),"%d",&options.blinkon);
			//if (options.blinkon < 1 || options.blinkon > 10000 )
				//sscanf(DEFAULT_OPTIONS_LEDS_BLINK_ON,"%d",&options.blinkon);
					//options.blinkon = (int *)DEFAULT_OPTIONS_LEDS_BLINK_ON;
		}
		else if (strcmp(tmp,"OPTIONS_LEDS_BLINK_OFF")==0) {
			sscanf(strtok(NULL,"\0"),"%d",&options.blinkoff);
			//if (options.blinkoff < 1 || options.blinkoff > 10000)
				//sscanf(DEFAULT_OPTIONS_LEDS_BLINK_OFF,"%d",&options.blinkoff);
		}
		//else if (strcmp(tmp,"OPTIONS_LEDS_BLINK_OFF")!=0) {
		//		sscanf(DEFAULT_OPTIONS_LEDS_BLINK_OFF,"%d",&options.blinkoff);
		//}

        /*
         * 2. the function of LED rows 'in' and 'out'
         * we can for every row on the front define
         *  1. what device and
         *  2. what direction on this device
         *  3. with what tranfer rate as maximum
         * should be shown
         * for example: in '/proc/mct/led/in' is written 'eth0 r 100'
         * this mean: the 'in' row shows the device eth0, all (r)eceived packets with 100kB/s as maximum (all 4 LEDs are on!)
         */
        // values for the 'IN' row
        else if (strcmp(tmp,"OPTIONS_LEDS_IN_DEVICE")==0) { // the 'in' row in the front
            options.led_in_device=Strdup(strtok(NULL,"\0"));
            options.led_in_device[strlen(options.led_in_device)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_LEDS_IN_DIRECTION")==0) { // what direction should shown in the 'in' row? (r)eceice or (t)ransmit
            options.led_in_direction=Strdup(strtok(NULL,"\0"));
            options.led_in_direction[strlen(options.led_in_direction)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_LEDS_IN_INTERVAL")==0) { // what is the maximum in kB/s
            sscanf(strtok(NULL,"\0"),"%d",&options.led_in_interval);
        }
        // values for the 'OUT' row
        else if (strcmp(tmp,"OPTIONS_LEDS_OUT_DEVICE")==0) { // the 'in' row in the front
            options.led_out_device=Strdup(strtok(NULL,"\0"));
            options.led_out_device[strlen(options.led_out_device)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_LEDS_OUT_DIRECTION")==0) { // what direction should shown in the 'in' row? (r)eceice or (t)ransmit
            options.led_out_direction=Strdup(strtok(NULL,"\0"));
            options.led_out_direction[strlen(options.led_out_direction)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_LEDS_OUT_INTERVAL")==0) { // what is the maximum in kB/s
            sscanf(strtok(NULL,"\0"),"%d",&options.led_out_interval);
        }
        // ethtool for eth0
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH0_DUPLEX")==0) { // HD or VD on eth0
            options.ethtool_eth0_dm=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth0_dm[strlen(options.ethtool_eth0_dm)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH0_AUTONEG")==0) { // Autonegotation on eth0 on or off
            options.ethtool_eth0_nego=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth0_nego[strlen(options.ethtool_eth0_nego)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH0_SPEED")==0) { // 100MBit or 10MBit
            sscanf(strtok(NULL,"\0"),"%d",&options.ethtool_eth0_speed);
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH0_WAKEONLAN")==0) { // WakeonLan
            options.ethtool_eth0_wol=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth0_wol[strlen(options.ethtool_eth0_wol)-1]='\0';
        }
        // ethtool for eth1
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH1_DUPLEX")==0) { // HD or VD on eth0
            options.ethtool_eth1_dm=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth1_dm[strlen(options.ethtool_eth1_dm)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH1_AUTONEG")==0) { // Autonegotation on eth1 on or off
            options.ethtool_eth1_nego=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth1_nego[strlen(options.ethtool_eth1_nego)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH1_SPEED")==0) { // 100MBit or 10MBit
            sscanf(strtok(NULL,"\0"),"%d",&options.ethtool_eth1_speed);
        }
        else if (strcmp(tmp,"OPTIONS_ETHTOOL_ETH1_WAKEONLAN")==0) { // WakeonLan
            options.ethtool_eth1_wol=Strdup(strtok(NULL,"\0"));
            options.ethtool_eth1_wol[strlen(options.ethtool_eth1_wol)-1]='\0';
        }
        // NTP Date Server IP
        else if (strcmp(tmp,"OPTIONS_NTPDATE_SERVER")==0) {
            options.ntpip=Strdup(strtok(NULL,"\0"));
            options.ntpip[strlen(options.ntpip)-1]='\0';
        }
        // IP Forwarding
        else if (strcmp(tmp,"ENABLE_IPFORWARDING")==0) {
            options.ip_forwarding=Strdup(strtok(NULL,"\0"));
            options.ip_forwarding[strlen(options.ip_forwarding)-1]='\0';
        }
        // the harddisk parameters
#warning "need to check for partitions!"        /* FIXME! we have to check that the hd has partitions ! */
        else if (strcmp(tmp,"OPTIONS_HDPARM_DEVICE")==0) {
            options.hd_device=Strdup(strtok(NULL,"\0"));
            options.hd_device[strlen(options.hd_device)-1]='\0';
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_TRANSFER_MODE")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_transfer_mode);
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_DMA")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_dma);
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_INTERRUPT_UNMASK")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_interrupt_umask);
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_SECTOR_COUNT")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_sector_count);
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_32BIT")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_32bit);
        }
        else if (strcmp(tmp,"OPTIONS_HDPARM_SPINDOWN_TIMEOUT")==0) {
            sscanf(strtok(NULL,"\0"),"%d",&options.hd_spindown);
        }
        tmp=NULL;
    }

	/* the sanity checks! */
	if (options.blinkon < 1 || options.blinkon > 10000)
		sscanf(DEFAULT_OPTIONS_LEDS_BLINK_ON,"%d",&options.blinkon);
	if (options.blinkoff < 1 || options.blinkoff > 10000)
		sscanf(DEFAULT_OPTIONS_LEDS_BLINK_OFF,"%d",&options.blinkoff);
	if (options.led_in_interval < 1 || options.led_in_interval > 1000)
		sscanf(DEFAULT_OPTIONS_LEDS_IN_INTERVAL,"%d",&options.led_in_interval);
	if (options.led_out_interval < 1 || options.led_out_interval > 1000)
		sscanf(DEFAULT_OPTIONS_LEDS_OUT_INTERVAL,"%d",&options.led_out_interval);
	if (options.ethtool_eth0_speed != 10 || options.ethtool_eth0_speed != 100)
		sscanf(DEFAULT_OPTIONS_ETHTOOL_ETH0_SPEED,"%d",&options.ethtool_eth0_speed);
	if (options.ethtool_eth1_speed != 10 || options.ethtool_eth1_speed != 100)
		sscanf(DEFAULT_OPTIONS_ETHTOOL_ETH1_SPEED,"%d",&options.ethtool_eth1_speed);
	if (options.hd_transfer_mode != 33 || options.hd_transfer_mode != 66)
		sscanf(DEFAULT_OPTIONS_HDPARM_TRANSFER_MODE,"%d",&options.hd_transfer_mode);
	if (options.hd_dma != 0 || options.hd_dma != 1)
		sscanf(DEFAULT_OPTIONS_HDPARM_DMA,"%d",&options.hd_dma);
	if (options.hd_interrupt_umask != 0 || options.hd_interrupt_umask != 1)
		sscanf(DEFAULT_OPTIONS_HDPARM_INTERRUPT_UNMASK,"%d",&options.hd_interrupt_umask);
	if (options.hd_sector_count != 2 ||
		options.hd_sector_count != 4 ||
		options.hd_sector_count != 8 ||
		options.hd_sector_count != 16)
		sscanf(DEFAULT_OPTIONS_HDPARM_SECTOR_COUNT,"%d",&options.hd_sector_count);
	if (options.hd_32bit != 0 || options.hd_32bit != 2 || options.hd_32bit != 3)
		sscanf(DEFAULT_OPTIONS_HDPARM_32BIT,"%d",&options.hd_32bit);
	if (options.hd_spindown < 1 || options.hd_spindown > 255)
		sscanf(DEFAULT_OPTIONS_HDPARM_SPINDOWN_TIMEOUT,"%d",&options.hd_spindown);

#ifdef DEBUG1
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d]\n"
                                                      "value rc_start.crond:\t\t'%s'\n"
                                                      "value rc_start.ethtool_eth0:\t'%s'\n"
                                                      "value rc_start.ethtool_eth1:\t'%s'\n"
                                                      "value rc_start.ftpd:\t\t'%s'\n"
                                                      "value rc_start.hdparm:\t\t'%s'\n"
                                                      "value rc_start.httpd:\t\t'%s'\n"
                                                      "value rc_start.leds:\t\t'%s'\n"
                                                      "value rc_start.nfs:\t\t'%s'\n"
                                                      "value rc_start.ntpdate:\t\t'%s'\n"
                                                      "value rc_start.portmap:\t\t'%s'\n"
                                                      "value rc_start.samba:\t\t'%s'\n"
                                                      "value rc_start.smartd:\t\t'%s'\n"
                                                      "value rc_start.sshd:\t\t'%s'\n"
                                                      "value rc_start.swap:\t\t'%s'\n"
                                                      "value rc_start.syslog:\t\t'%s'\n"
                                                      "value rc_start.telnetd:\t\t'%s'\n"
                                                      "value rc_start.vservers:\t'%s'\n"
                                                      ,__FUNCTION__
                                                      ,__LINE__
                                                      ,rc_start.crond
                                                      ,rc_start.ethtool_eth0
                                                      ,rc_start.ethtool_eth1
                                                      ,rc_start.ftpd
                                                      ,rc_start.hdparm
                                                      ,rc_start.httpd
                                                      ,rc_start.leds
                                                      ,rc_start.nfs
                                                      ,rc_start.ntpdate
                                                      ,rc_start.portmap
                                                      ,rc_start.samba
                                                      ,rc_start.smartd
                                                      ,rc_start.sshd
                                                      ,rc_start.swap
                                                      ,rc_start.syslogd
                                                      ,rc_start.telnetd
                                                      ,rc_start.vservers);
            debug_info(debug_buf);
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d]\n"
                                                      "value options.hb:\t\t'%s'\n"
													  "value options.blinkon:\t\t'%d'\n"
													  "value options.blinkoff:\t\t'%d'\n"
                                                      "value options.led_in_device:\t'%s'\n"
                                                      "value options.led_in_direction:\t'%s'\n"
                                                      "value options.led_in_interval:\t'%d'\n"
                                                      "value options.led_out_device:\t'%s'\n"
                                                      "value options.led_out_direction:'%s'\n"
                                                      "value options.led_out_interval:\t'%d'\n"
                                                      "value options.ethtool_eth0_dm:\t'%s'\n"
                                                      "value options.ethtool_eth0_nego: '%s'\n"
                                                      "value options.ethtool_speed_dm:\t'%d'\n"
                                                      "value options.ethtool_eth0_wol:\t'%s'\n"
                                                      "value options.ethtool_eth1_dm:\t'%s'\n"
                                                      "value options.ethtool_eth1_nego: '%s'\n"
                                                      "value options.ethtool_eth1_speed:'%d'\n"
                                                      "value options.ethtool_eth1_wol:\t'%s'\n"
                                                      ,__FUNCTION__
                                                      ,__LINE__
                                                      ,options.hb
													  ,options.blinkon
													  ,options.blinkoff
                                                      ,options.led_in_device
                                                      ,options.led_in_direction
                                                      ,options.led_in_interval
                                                      ,options.led_out_device
                                                      ,options.led_out_direction
                                                      ,options.led_out_interval
                                                      ,options.ethtool_eth0_dm
                                                      ,options.ethtool_eth0_nego
                                                      ,options.ethtool_eth0_speed
                                                      ,options.ethtool_eth0_wol
                                                      ,options.ethtool_eth1_dm
                                                      ,options.ethtool_eth1_nego
                                                      ,options.ethtool_eth1_speed
                                                      ,options.ethtool_eth1_wol);
            debug_info(debug_buf);
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d]\n"
                                                      "value options.ntpip:\t\t'%s'\n"
                                                      "value options.hd_device:\t'%s'\n"
                                                      "value options.hd_transfer_mode:\t'%d'\n"
                                                      "value options.hd_dma:\t\t'%d'\n"
                                                      "value options.hd_interrupt_umask:'%d'\n"
                                                      "value options.hd_sector_count:\t'%d'\n"
                                                      "value options.hd_32bit:\t\t'%d'\n"
                                                      "value options.hd_spindown:\t'%d'\n"
                                                      ,__FUNCTION__
                                                      ,__LINE__
                                                      ,options.ntpip
                                                      ,options.hd_device
                                                      ,options.hd_transfer_mode
                                                      ,options.hd_dma
                                                      ,options.hd_interrupt_umask
                                                      ,options.hd_sector_count
                                                      ,options.hd_32bit
                                                      ,options.hd_spindown);
            debug_info(debug_buf);
#endif

    fclose(rcconf);
    return 1;
}

/* function: write the data values to /var/etc/rc.conf
 * return 0 if success
 * we getting the data from owi.c from the struct *cgi
 */
int write_rcconf(struct CGI_DATA *data) {

    FILE *conf=NULL;
    char *tmp=NULL;

    if ((conf = fopen(NEW_RC_CONFIG_FILE, "w")) == NULL)
        return 1;   // return with status 1 'could't open tempfile'
    else {
        snprintf(debug_buf,sizeof(debug_buf)-1,"[%s/%d] open %s o.k.\n",__FUNCTION__,__LINE__,NEW_RC_CONFIG_FILE);
        debug_info(debug_buf);
    }
    // writing the header to the new rc.conf file
    fprintf(conf,"# rc.conf created %s by %s %s \n",__DATE__,PACKAGE_NAME,OWI_VERSION);
    fprintf(conf,"# OpenMCT Confguration File, edit this file carefully!\n"
				 "#\n"
				 // the boot up starting services
				 "############ start section services ############\n");
	// (in alphabetical order) crond
    tmp=(read_cgi_entry(data,"crond"));
    if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_CROND=%s\n",DEFAULT_START_CROND);
    else
        fprintf(conf,"START_CROND=%s\n",tmp);
	// ETHTool for ETH0
    tmp=(read_cgi_entry(data,"etool_eth0"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_ETHTOOL_ETH0=%s\n",DEFAULT_START_ETHTOOL_ETH0);
    else
        fprintf(conf,"START_ETHTOOL_ETH0=%s\n",tmp);
    // ETHTool for ETH1
    tmp=(read_cgi_entry(data,"etool_eth1"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_ETHTOOL_ETH1=%s\n",DEFAULT_START_ETHTOOL_ETH1);
    else
        fprintf(conf,"START_ETHTOOL_ETH1=%s\n",tmp);
    // ftpd
    tmp=(read_cgi_entry(data,"ftp"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_FTPD=%s\n",DEFAULT_START_FTPD);
    else
        fprintf(conf,"START_FTPD=%s\n",tmp);
	// hdparm
	tmp=(read_cgi_entry(data,"hdparm"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_HDPARM=%s\n",DEFAULT_START_HDPARM);
    else
        fprintf(conf,"START_HDPARM=%s\n",tmp);
	// httpd
	tmp=(read_cgi_entry(data,"http"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_HTTPD=%s\n",DEFAULT_START_HTTPD);
    else
        fprintf(conf,"START_HTTPD=%s\n",tmp);
	// LEDs
	tmp=(read_cgi_entry(data,"led"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_LEDS=%s\n",DEFAULT_START_LEDS);
    else
        fprintf(conf,"START_LEDS=%s\n",tmp);
	// NFS
	tmp=(read_cgi_entry(data,"nfs"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_NFS=%s\n",DEFAULT_START_NFS);
    else
        fprintf(conf,"START_NFS=%s\n",tmp);
	// NTPDate
	tmp=(read_cgi_entry(data,"ntpdate"));
	if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_NTPDATE=%s\n",DEFAULT_START_NTPDATE);
    else
        fprintf(conf,"START_NTPDATE=%s\n",tmp);
	// Portmap
	tmp=(read_cgi_entry(data,"portmap"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_PORTMAP=%s\n",DEFAULT_START_PORTMAP);
    else
        fprintf(conf,"START_PORTMAP=%s\n",tmp);
	// Samba
	tmp=(read_cgi_entry(data,"samba"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_SAMBA=%s\n",DEFAULT_START_SAMBA);
    else
        fprintf(conf,"START_SAMBA=%s\n",tmp);
	// SMARTD
	tmp=(read_cgi_entry(data,"smartd"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_SMARTD=%s\n",DEFAULT_START_SMARTD);
    else
        fprintf(conf,"START_SMARTD=%s\n",tmp);
	// SSH
	tmp=(read_cgi_entry(data,"ssh"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_SSHD=%s\n",DEFAULT_START_SSHD);
    else
        fprintf(conf,"START_SSHD=%s\n",tmp);
	// Swap
	tmp=(read_cgi_entry(data,"swap"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_SWAP=%s\n",DEFAULT_START_SWAP);
    else
        fprintf(conf,"START_SWAP=%s\n",tmp);
	// Syslog
	tmp=(read_cgi_entry(data,"syslog"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_SYSLOGD=%s\n",DEFAULT_START_SYSLOGD);
    else
        fprintf(conf,"START_SYSLOGD=%s\n",tmp);
	// Telnet
	tmp=(read_cgi_entry(data,"telnet"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_TELNETD=%s\n",DEFAULT_START_TELNETD);
    else
        fprintf(conf,"START_TELNETD=%s\n",tmp);
	// Vserver
	tmp=(read_cgi_entry(data,"vserver"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"START_VSERVERS=%s\n",DEFAULT_START_VSERVERS);
    else
        fprintf(conf,"START_VSERVERS=%s\n",tmp);
	// end of the boot up starting services
	fprintf(conf,"############ stop section services ############\n#\n");
	// led section
	fprintf(conf,"############ start section leds ############\n");
	// Heartbeat LED
	tmp=(read_cgi_entry(data,"hb"));
    if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_HB=%s\n",DEFAULT_OPTIONS_HB);
    else
        fprintf(conf,"OPTIONS_LED_HB=%s\n",tmp);
	// if Heartbeat LED is set to blink
	tmp=(read_cgi_entry(data,"blinkon"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_BLINK_ON=%s\n",DEFAULT_OPTIONS_LEDS_BLINK_ON);
    else
        fprintf(conf,"OPTIONS_LED_BLINK_ON=%s\n",tmp);
	tmp=(read_cgi_entry(data,"blinkoff"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_BLINK_OFF=%s\n",DEFAULT_OPTIONS_LEDS_BLINK_OFF);
    else
        fprintf(conf,"OPTIONS_LED_BLINK_OFF=%s\n",tmp);
	// Device for Traffic LED row 'IN'
	tmp=(read_cgi_entry(data,"led_in_dev"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_IN_DEVICE=%s\n",DEFAULT_OPTIONS_LEDS_IN_DEVICE);
    else
        fprintf(conf,"OPTIONS_LED_IN_DEVICE=%s\n",tmp);
	// Direction for Traffic LED row 'IN'
	tmp=(read_cgi_entry(data,"led_in_dir"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_IN_DIRECTION=%s\n",DEFAULT_OPTIONS_LEDS_IN_DIRECTION);
    else
        fprintf(conf,"OPTIONS_LED_IN_DIRECTION=%s\n",tmp);
	// Maximum for Traffic LED row 'IN'
	tmp=(read_cgi_entry(data,"led_in_interval"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_IN_INTERVAL=%s\n",DEFAULT_OPTIONS_LEDS_IN_INTERVAL);
    else
        fprintf(conf,"OPTIONS_LED_IN_INTERVAL=%s\n",tmp);
	// Device for Traffic LED row 'OUT'
	tmp=(read_cgi_entry(data,"led_out_dev"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_OUT_DEVICE=%s\n",DEFAULT_OPTIONS_LEDS_OUT_DEVICE);
    else
        fprintf(conf,"OPTIONS_LED_OUT_DEVICE=%s\n",tmp);
	// Direction for Traffic LED row 'OUT'
	tmp=(read_cgi_entry(data,"led_out_dir"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_OUT_DIRECTION=%s\n",DEFAULT_OPTIONS_LEDS_OUT_DIRECTION);
    else
        fprintf(conf,"OPTIONS_LED_OUT_DIRECTION=%s\n",tmp);
	// Maximum for Traffic LED row 'OUT'
	tmp=(read_cgi_entry(data,"led_out_interval"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_LED_OUT_INTERVAL=%s\n",DEFAULT_OPTIONS_LEDS_OUT_INTERVAL);
    else
        fprintf(conf,"OPTIONS_LED_OUT_INTERVAL=%s\n",tmp);
	// end of led section
	fprintf(conf,"############ end section leds ############\n#\n");

	// section swap partition
	fprintf(conf,"############ start section swap ############\n");
#warning "swap partition is static! we need to build a webpage to configure the hardisk handling!"
	fprintf(conf,"OPTIONS_SWAP_DEVICE=/dev/ide/host0/bus0/target0/lun0/part2\n");
	fprintf(conf,"############# end section swap #############\n#\n");

	// hdparm parameters
	fprintf(conf,"############ start section hdparm ############\n");
	fprintf(conf,"OPTIONS_HDPARM_DEVICE=/dev/ide/host0/bus0/target0/lun0/disc\n");
	// UDMA Mode
	tmp=(read_cgi_entry(data,"udma"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_TRANSFER_MODE=%s\n",DEFAULT_OPTIONS_HDPARM_TRANSFER_MODE);
    else
        fprintf(conf,"OPTIONS_HDPARM_TRANSFER_MODE=%s\n",tmp);
	// DMA Modus
	tmp=(read_cgi_entry(data,"dma"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_DMA=%s\n",DEFAULT_OPTIONS_HDPARM_DMA);
    else
        fprintf(conf,"OPTIONS_HDPARM_DMA=%s\n",tmp);
	// umask IRQ
	tmp=(read_cgi_entry(data,"umaskirq"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_INTERRUPT_UNMASK=%s\n",DEFAULT_OPTIONS_HDPARM_INTERRUPT_UNMASK);
    else
        fprintf(conf,"OPTIONS_HDPARM_INTERRUPT_UNMASK=%s\n",tmp);
	// sector count read/write
	tmp=(read_cgi_entry(data,"sector_count"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_SECTOR_COUNT=%s\n",DEFAULT_OPTIONS_HDPARM_SECTOR_COUNT);
    else
        fprintf(conf,"OPTIONS_HDPARM_SECTOR_COUNT=%s\n",tmp);
	// 32bit IO Modus
	tmp=(read_cgi_entry(data,"multi_io"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_32BIT=%s\n",DEFAULT_OPTIONS_HDPARM_32BIT);
    else
        fprintf(conf,"OPTIONS_HDPARM_32BIT=%s\n",tmp);
	// Sindown Timeout
	tmp=(read_cgi_entry(data,"hd_to"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_HDPARM_SPINDOWN_TIMEOUT=%s\n",DEFAULT_OPTIONS_HDPARM_SPINDOWN_TIMEOUT);
    else
        fprintf(conf,"OPTIONS_HDPARM_SPINDOWN_TIMEOUT=%s\n",tmp);
	// end of hdparm section
	fprintf(conf,"############# end section hdparm #############\n#\n");

	// NTP Date
	fprintf(conf,"############ start section ntpdate ############\n");
	tmp=(read_cgi_entry(data,"ntp"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_NTPDATE_SERVER=%s\n",DEFAULT_OPTIONS_NTPDATE_SERVER);
    else
        fprintf(conf,"OPTIONS_NTPDATE_SERVER=%s\n",tmp);
	fprintf(conf,"############# end section ntpdate #############\n#\n");

	// IP Forwarding
	fprintf(conf,"############ start section networking ############\n");
	tmp=(read_cgi_entry(data,"ip_forwarding"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"ENABLE_IPFORWARDING=%s\n",DEFAULT_ENABLE_IPFORWARDING);
    else
        fprintf(conf,"ENABLE_IPFORWARDING=%s\n",tmp);
	fprintf(conf,"############# end section networking #############\n#\n");

	// ETHTool
	fprintf(conf,"############ start section ethtool ############\n");
	// Duplex Modus eth0
	tmp=(read_cgi_entry(data,"eth0_duplex"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_DUPLEX=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH0_DUPLEX);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_DUPLEX=%s\n",tmp);
	// Autonegotiation Modus eth0
	tmp=(read_cgi_entry(data,"eth0_nego"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_AUTONEG=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH0_AUTONEG);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_AUTONEG=%s\n",tmp);
	// Speed Modus eth0
	tmp=(read_cgi_entry(data,"eth0_speed"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_SPEED=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH0_SPEED);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_SPEED=%s\n",tmp);
	// WakeOnLAN Modus eth0
	tmp=(read_cgi_entry(data,"eth0_wol"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_WAKEONLAN=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH0_WAKEONLAN);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH0_WAKEONLAN=%s\n",tmp);
	// Duplex Modus eth1
	tmp=(read_cgi_entry(data,"eth1_duplex"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_DUPLEX=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH1_DUPLEX);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_DUPLEX=%s\n",tmp);
	// Autonegotiation Modus eth1
	tmp=(read_cgi_entry(data,"eth1_nego"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_AUTONEG=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH1_AUTONEG);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_AUTONEG=%s\n",tmp);
	// Speed Modus eth1
	tmp=(read_cgi_entry(data,"eth1_speed"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_SPEED=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH1_SPEED);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_SPEED=%s\n",tmp);
	// WakeOnLAN Modus eth1
	tmp=(read_cgi_entry(data,"eth1_wol"));
        if(strcmp(tmp,"nothing")==0)
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_WAKEONLAN=%s\n",DEFAULT_OPTIONS_ETHTOOL_ETH1_WAKEONLAN);
    else
        fprintf(conf,"OPTIONS_ETHTOOL_ETH1_WAKEONLAN=%s\n",tmp);
	fclose(conf);
    // all entrys are written, now we can replace the new config file with the old
    return 0;
}

/* function: read a value from a given variable from cgi data out
 * return the value as char if found
 * return the string "nothing" if not found
 */
unsigned char *read_cgi_entry(struct CGI_DATA *data, char *variable) {

    int found=0, loop=1;
    extern char debug_buf[600];   // for debugging output

    while(data != NULL && found==0){
        if(strcmp(data->variable,variable)==0){
            found=1;
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] found requested value for %s = %s \n",__FUNCTION__,__LINE__,data->variable,data->value);
            debug_info(debug_buf);
            return data->value;
        }
        loop++;
        data=data->next;
    }
    if (data==NULL){
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] no entry for '%s' found, use default! \n",__FUNCTION__,__LINE__,variable);
        debug_info(debug_buf);
        return "nothing";
    }
    return "nothing"; //should never happen, just for secure!
}

