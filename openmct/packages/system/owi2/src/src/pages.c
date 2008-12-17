/* p_start.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 *
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 *
 * the default start page of the OWI (not the index.html! )
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "owi2.h"
#include "html.h"
#include "functions.h"

void start_page(char *title)
{
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page start template started\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    page_head(title);
    page_menu();
    page_content_start();
    printf("<h1 class=\"content_text\">Start</h1>\n");
    printf("<h2 class=\"content_text\">Bitte einen Men&#252punkt ausw&#228hlen!</h2>\n");
    printf("    <fieldset id=\"sysinfo\">\n");
    printf("    <legend> Systemstatus</legend>\n");
    printf("        <h3 class=\"content_text\">Festplatte\n"
           "		<img class=\"content_text\" border=\"0\" src=\"../img/hdd_unmount.png\">\n");
    if (strncmp(hd.modell,"SAM",3)==0) 		// Samsung
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/samsung_logo16.png\"/></h3>\n");
    else if (strncmp(hd.modell,"SAM",3)==0) // Western Digital
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/WD_logo16.png\"/></h3>\n");
    else if ( (strncmp(hd.modell,"IBM",3)==0) || (strncmp(hd.modell,"IC",2)==0)) // IBM
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/IBM_logo16.png\"/></h3>\n");
    else if (strncmp(hd.modell,"ST",2)==0) // Seagate
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/seagate_logo16.png\"/></h3>\n");
    else if (strncmp(hd.modell,"HTC",3)==0) // Hitachi
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/hitachi_logo16.png\"/></h3>\n");
    else if (strncmp(hd.modell,"Exc",3)==0) // Excelstore
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/excelstore_logo16.png\"/></h3>\n");
    else if (strncmp(hd.modell,"Max",3)==0) // Maxtor
        printf("<img class=\"content_text\" border=\"0\" src=\"../img/maxtor_logo16.png\"/></h3>\n");

    else
        printf("</h3>\n");
    printf("        <table class=\"content_text\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">\n");
    printf("        <tr>\n");
    printf("        	<td align=\"left\"><p class=\"content_text\"><b>Hersteller/Modell:</b></td><td> %s </td>"
           "<td><b>Firmware:</b></td><td> %s </td>"
           "<td><b>Seriennummer:</b></td><td> %s</td></p>"
           "		</tr>\n"
           ,hd.modell,hd.fw,hd.serial);
    printf("        <tr>\n"
           "			<td align=\"left\"><p class=\"content_text\"><b>Transfermodus:</b></td><td> %s </td>"
           "<td><b>Cachegr&#244&#223e:</b></td><td> %dkB </td>"
           "<td><b>Sektoren/Interrupt:</b></td><td> %d </td></p>\n"
           "		</tr>\n"
           ,hd.tfmode,hd.cache,hd.maxscset);
    printf("		<tr>\n");
    printf("			<td align=\"left\"><p class=\"content_text\"><b>Kapazit&#228t netto:</b></td><td>%dGB</td>"
           "<td><b>Kapazit&#228t brutto:</b></td><td>%dGB</td></p>\n"
           "		</tr>\n</table>",hd.size_netto,hd.size_brutto);

    // smartctl data
    printf("        <table class=\"content_text\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\">\n");
    printf("        <tr>\n");
    printf("        	<td align=\"left\"><p class=\"content_text\"><b>Status</b></td><td><b>SMART ID</b></td><td><b>Attribut</b></td>"
           "<td><b>aktueller Wert</b></td><td><b>Schwellwert</b></td>"
           "<td><b>Type</b></td><td><b>Update</b></td><td><b>RAW Wert</b></td></p>"
           "		</tr>\n");

// smartctl for info about hd fitness
    system("smartctl -A "HARDDISK" > /tmp/smartctl.tmp");
    FILE *i = fopen("/tmp/smartctl.tmp", "r");

    if (i)
    {
        unsigned char buffer[4096];
        unsigned char attribut[40], tmp[10], type[10], update[10], failed[20], raw_val[20];
        int id,val,worst,thresh;
		char *grafic=NULL;

        while (fgets(buffer, 4096, i))
        {
            memset(attribut, 0, sizeof(attribut));
            memset(type, 0, sizeof(type));
            memset(update, 0, sizeof(update));
            memset(failed, 0, sizeof(failed));
            memset(raw_val, 0, sizeof(raw_val));
            id=val=worst=thresh=0;

            if (sscanf(buffer, " %d %s                      %s       %d     %d    %d    %s        %s           %s       %s"
                       ,  &id, attribut,              tmp,     &val,  &worst,&thresh,type,    update,      failed,  raw_val) == 10)
            {
				if (val > thresh*100/110) // if value is greater then 110% of thresh every thing is o.k.
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/ok16x16.png\"/>";
				else if (val < thresh*100/110) // if value is less then 110% of thresh you should be careful, better make a backup
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/warning16x16.png\"/>";
				else if (val < thresh) // if value is less then 100% it is really time to make a backup!
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/no16x16.png\"/>";
				else
					grafic=" - ";

				// hd temperature is a special case
				if ( (id==194) && (atoi(raw_val) < 45) ) // if temperature is less then 45°C
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/ok16x16.png\"/>";
				else if ( (id==194) && (atoi(raw_val) > 45) ) // if temperature is less then 45°C
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/warning16x16.png\"/>";
				else if ( (id==194) && (atoi(raw_val) > 55) ) // if temperature is less then 55°C
					grafic="<img class=\"content_text\" border=\"0\" src=\"../img/no16x16.png\"/>";

				if (id!=190) // ID 190 is a manufactur special value and not temperature or something else!
				{
					printf( "        	<td align=\"center\"><p class=\"content_text\">%s</td><td><b>%d</b></td><td><b>%s</b></td>"
							"<td><b>%d</b></td><td><b>%d</b></td>"
							"<td><b>%s</b></td><td><b>%s</b></td><td><b>%s</b></td></p>"
							"		</tr>\n"
							,grafic,id, attribut,val,thresh,type,update,raw_val);
				}
            }
        }
        fclose(i);
        printf("        </table>\n");
    }
    else
    {
        printf("        <tr><td> keine S.M.A.R.T Daten!!</td></tr>\n");
        printf("        </table>\n");
    }

    printf("    </fieldset>\n");
    printf("    <fieldset id=\"sysinfo\">\n");
    printf("    <legend>Netzwerk</legend>\n");
    printf("        <h3 class=\"content_text\">Netzwerkstatus</h3>\n");
    printf("	</fieldset>\n");

    page_content_end();
    page_footer();
#ifdef DEBUG
    //extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page start template ended\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
}

void glob_set(char *title)
{
    page_head(title);
    page_menu();
    page_content_start();
    char *check=NULL;
#ifdef DEBUG
    extern char debug_buf[600];   // for debugging output
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page glob_set start\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    printf("<h1 class=\"content_text\">Einstellungen der zu startenden Dienste beim Booten des NAS</h1>\n");
    printf("<form action=\"../cgi-bin/owi\" method=\"post\">\n");
    printf("    <fieldset id=\"field12\">\n");
    printf("    <legend> Hauptdienste</legend>\n");
    printf("        <h3 class=\"content_text\">Start/Stop Filetranfer-Dienste:</h3>\n");
    printf("        <p class=\"content_text\">\n");
    printf("        <input type=\"hidden\" name=\"modul\" value=\"write-global-set\">\n");  // set value to 'write-gloabl-set'
    if (strcmp(rc_start.nfs,"yes")==0) check="checked=\"checked\"";
    else check=" ";
    printf("        <input type=\"checkbox\" name=\"nfs\" value=\"yes\" %s>NFS\n",check);
    if (strcmp(rc_start.samba,"yes")==0) check="checked=\"checked\"";
    else check=" ";
    printf("        <input type=\"checkbox\" name=\"samba\" value=\"yes\" %s>Samba\n",check);
    if (strcmp(rc_start.ftpd,"yes")==0) check="checked=\"checked\"";
    else check=" ";
    printf("        <input type=\"checkbox\" name=\"ftp\" value=\"yes\" %s>FTP</p>\n",check);

    printf("        <h3 class=\"content_text\">Start/Stop Fernzugangs-Dienste:</h3>\n");
    printf("        <p class=\"content_text\">\n");
    if (strcmp(rc_start.sshd,"yes")==0) check="checked=\"checked\"";
    else check='\0';
    printf("        <input type=\"checkbox\" name=\"ssh\" value=\"yes\" %s>SSH\n",check);
    if (strcmp(rc_start.telnetd,"yes")==0) check="checked=\"checked\"";
    else check='\0';
    printf("        <input type=\"checkbox\" name=\"telnet\" value=\"yes\" %s>Telnet</p>\n",check);
    printf("        <h3 class=\"content_text\">Start/Stop System-Dienste:</h3>\n");
    printf("        <p class=\"content_text\">\n");
    if (strcmp(rc_start.portmap,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"portmap\"   value=\"yes\" %s>Portmap\n",check);
    if (strcmp(rc_start.syslogd,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"syslog\"    value=\"yes\" %s>Syslog\n",check);
    if (strcmp(rc_start.ntpdate,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"ntpdate\"   value=\"yes\" %s>NTPDate\n",check);
    if (strcmp(rc_start.crond,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"cron\"      value=\"yes\" %s>Cron\n",check);
    if (strcmp(rc_start.leds,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"led\"      value=\"yes\" %s>LEDs\n",check);
    if (strcmp(rc_start.hdparm,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"hdparm\"    value=\"yes\" %s>HDParm\n",check);
    if (strcmp(rc_start.swap,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"swap\"      value=\"yes\" %s>Swap\n",check);
    if (strcmp(rc_start.httpd,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"http\"      value=\"yes\" %s>HTTP<br>\n",check);
    if (strcmp(rc_start.ethtool_eth0,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"etool_eth0\" value=\"yes\"%s >ETHTool f&#252r ETH0 <small>(WAN Port)</small>\n",check);
    if (strcmp(rc_start.ethtool_eth1,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"etool_eth1\" value=\"yes\"%s >ETHTool f&#252r ETH1 <small>(Switch Ports)</small>\n",check);
    if (strcmp(rc_start.smartd,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"smartd\"      value=\"yes\" %s>Smartd\n",check);
    if (strcmp(rc_start.vservers,"yes")==0) check="checked=\"checked\"";
    else check=" ";;
    printf("        <input type=\"checkbox\" name=\"vserver\"      value=\"yes\" %s>Vserver</p>\n",check);
    printf("    </fieldset>\n");
    printf("<fieldset>\n");

    printf("    <legend> Optionen f&#252r LED Anzeigen</legend>\n");
    // heartbeat LED //
    printf("        <table class=\"content_text\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">\n");
    printf("        <tr>\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Anzeige der Heartbeat LED</b></td>\n");
    printf("            <td><select name=\"hb\" size=\"1\">\n");
    if (strcmp(options.hb,"cpu")==0) check="selected";
    else check=" ";
    printf("                <option value=\"cpu\" %s>CPU Aktivit&#228t</option>\n",check);
    if (strcmp(options.hb,"on")==0) check="selected";
    else check=" ";
    printf("                <option value=\"on\" %s>dauer an</option>\n",check);
    if (strcmp(options.hb,"off")==0) check="selected";
    else check=" ";
    printf("                <option value=\"off\" %s>dauer aus</option>\n",check);
    if (strcmp(options.hb,"blink")==0) check="selected";
    else check=" ";
    printf("                <option value=\"blink\" %s>blinkend</option>\n",check);
    printf("            </select>\n");
    printf("			<td>Blinkdauer an</td><td><input name=\"blinkon\" type=\"text\" size=\"3\" maxlength=\"3\" value=%d>\n",options.blinkon);
    printf("			<td>Blinkdauer aus</td><td><input name=\"blinkoff\" type=\"text\" size=\"3\" maxlength=\"3\" value=%d></p></tr>\n",options.blinkoff);
    // LED IN row //
    printf("        <tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Anzeige LED Reihe <b>'IN'</b></td>\n");
    printf("            <td><select name=\"led_in_dev\" size=\"1\">\n");
    if (strcmp(options.led_in_device,"eth0")==0) check="selected";
    else check=" ";
    printf("                <option value=\"eth0\" %s>eth0 (WAN Port)</option>\n",check);
    if (strcmp(options.led_in_device,"eth1")==0) check="selected";
    else check=" ";
    printf("                <option value=\"eth1\" %s>eth1 (Switch Ports)</option>\n",check);
    printf("            </select></td>\n");
    printf("        </tr>");

    printf("        <tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Traffic LED Reihe <b>'IN'</b></td>\n");
    printf("            <td><select name=\"led_in_dir\" size=\"1\">\n");
    if (strcmp(options.led_in_direction,"r")==0) check="selected";
    else check=" ";
    printf("                <option value=\"r\" %s>eingehend (receive)</option>\n",check);
    if (strcmp(options.led_in_direction,"t")==0) check="selected";
    else check=" ";
    printf("                <option value=\"t\" %s>ausgehend (transmit)</option>\n",check);
    printf("            </select></p></td>\n");
    printf("        <tr\n");

    printf("        </tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Maximum in kB f&#252r <b>'IN'</b></td>\n");
    printf("            <td><input name=\"led_in_interval\" type=\"text\" size=\"3\" maxlength=\"3\" value=%d></p></td>\n",options.led_in_interval);
    printf("        </tr\n");

    // OUT row //
    printf("        <tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Anzeige LED Reihe <b>'OUT'</b></td>\n");
    printf("            <td><select name=\"led_out_dev\" size=\"1\">\n");
    if (strcmp(options.led_out_device,"eth0")==0) check="selected";
    else check=" ";
    printf("                <option value=\"eth0\" %s>eth0 (WAN Port)</option>\n",check);
    if (strcmp(options.led_out_device,"eth1")==0) check="selected";
    else check=" ";
    printf("                <option value=\"eth1\" %s>eth1 (Switch Ports)</option>\n",check);
    printf("            </select></td>\n");
    printf("        </tr\n");

    printf("        <tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Traffic LED Reihe <b>'OUT'</b></td>\n");
    printf("            <td><select name=\"led_out_dir\" size=\"1\">\n");
    if (strcmp(options.led_out_direction,"r")==0) check="selected";
    else check=" ";
    printf("                <option value=\"r\" %s>eingehend (receive)</option>\n",check);
    if (strcmp(options.led_out_direction,"t")==0) check="selected";
    else check=" ";
    printf("                <option value=\"t\" %s>ausgehend (transmit)</option>\n",check);
    printf("            </select></p></td>\n");
    printf("        </tr\n");

    printf("        <tr\n");
    printf("            <td align=\"right\"><p class=\"content_text\" >Maximum in kB f&#252r <b>'OUT'</b></td>\n");
    printf("            <td><input name=\"led_out_interval\" type=\"text\" size=\"3\" maxlength=\"3\" value=%d></p></td>\n",options.led_out_interval);
    printf("        </tr\n");
    printf("        </table>");
    printf("    </fieldset>\n");

    printf("<fieldset>\n");
    printf("    <legend>Einstellungen ethTool</legend>\n");
    printf("        <h3 class=\"content_text\">Einstellungen f&#252r ETH0 <small>(WAN Port)</small></h3>\n");
    printf("        <table class=\"content_text\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">\n");
    printf("        <tr>\n");
    printf("            <td>Duplexmodus</td>\n");
    if (strcmp(options.ethtool_eth0_dm,"half")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_duplex\" value=\"half\" %s>HalbDuplex</td>\n",check);
    if (strcmp(options.ethtool_eth0_dm,"full")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_duplex\" value=\"full\" %s>FullDuplex</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("            <td>Autonegotion</td>\n");
    if (strcmp(options.ethtool_eth0_nego,"on")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_nego\" value=\"on\" %s>an</td>\n",check);
    if (strcmp(options.ethtool_eth0_nego,"off")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_nego\" value=\"off\" %s>aus</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("            <td>Geschwindigkeit</td>\n");
    if (options.ethtool_eth0_speed==10) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_speed\" value=\"10\" %s> 10MBit</td>\n",check);
    if (options.ethtool_eth0_speed==100) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth0_speed\" value=\"100\" %s> 100MBit</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("            <td>Wake On LAN</td>\n");

#warning "The Wake On LAN can more modi then the default 'bg' !"

    if (strcmp(options.ethtool_eth0_wol,"bg")==0)   // FixME!! there are more modi then 'bg'
    {
        printf("            <td><input type=\"radio\" name=\"eth0_wol\" value=\"bg\" checked=\"checked\"> an</td>\n");
        printf("            <td><input type=\"radio\" name=\"eth0_wol\" value=\"d\" > aus</td>\n");
    }
    else
    {
        printf("            <td><input type=\"radio\" name=\"eth0_wol\" value=\"bg\"> an</td>\n");
        printf("            <td><input type=\"radio\" name=\"eth0_wol\" value=\"d\" checked=\"checked\"> aus</td>\n");
    }
    printf("        </tr>\n");
    printf("        </table>\n");

    printf("        <h3 class=\"content_text\">Einstellungen f&#252r ETH1 <small>(Switch Ports)</small></h3>\n");
    printf("        <table class=\"content_text\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\">\n");
    printf("        <tr>\n");
    printf("            <td>Duplexmodus</td>\n");
    if (strcmp(options.ethtool_eth1_dm,"half")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_duplex\" value=\"half\" %s>HalbDuplex</td>\n",check);
    if (strcmp(options.ethtool_eth1_dm,"full")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_duplex\" value=\"full\" %s>FullDuplex</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("            <td>Autonegotion</td>\n");
    if (strcmp(options.ethtool_eth1_nego,"on")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_nego\" value=\"on\" %s>an</td>\n",check);
    if (strcmp(options.ethtool_eth1_nego,"off")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_nego\" value=\"off\" %s>aus</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("        <td>Geschwindigkeit</td>\n");
    if (options.ethtool_eth1_speed==10) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_speed\" value=\"10\" %s> 10MBit</td>\n",check);
    if (options.ethtool_eth1_speed==100) check="checked=\"checked\"";
    else check=" ";
    printf("            <td><input type=\"radio\" name=\"eth1_speed\" value=\"100\" %s> 100MBit</td>\n",check);
    printf("        </tr>\n");
    printf("        <tr>\n");
    printf("            <td>Wake On LAN</td>\n");
    if (strcmp(options.ethtool_eth1_wol,"bg")==0)   // FixME!! there are more modi then 'bg'
    {
        printf("            <td><input type=\"radio\" name=\"eth1_wol\" value=\"bg\" checked=\"checked\">an</td>\n");
        printf("            <td><input type=\"radio\" name=\"eth1_wol\" value=\"d\" >aus</td>\n");
    }
    else
    {
        printf("            <td><input type=\"radio\" name=\"eth1_wol\" value=\"bg\">an</td>\n");
        printf("            <td><input type=\"radio\" name=\"eth1_wol\" value=\"d\" checked=\"checked\">aus</td>\n");
    }
    printf("        </tr>\n");
    printf("        </table>\n");
    printf("    </fieldset>\n");

    printf("    <fieldset>\n");
    printf("    <legend> Diverses</legend>\n");
    printf("        <h3 class=\"content_text\">NTPDate</h3>\n");
    printf("            <p class=\"content_text\" >NTPDate Server IP\n");
    printf("            <select name=\"ntp\" size=\"1\">\n");
    if (strcmp(options.ntpip,"192.53.103.104")==0) check="selected";
    else check=" ";
    printf("                <option value=\"192.53.103.104\" %s>DTAG (192.53.103.104)</option>\n",check);
    if (strcmp(options.ntpip,"134.130.4.17")==0) check="selected";
    else check=" ";
    printf("                <option value=\"134.130.4.17\" %s>RWTH Aachen (134.130.4.17)</option>\n",check);
    if (strcmp(options.ntpip,"131.188.3.221")==0) check="selected";
    else check=" ";
    printf("                <option value=\"131.188.3.221\" %s>Uni Erlangen (131.188.3.221)</option>\n",check);
    if (strcmp(options.ntpip,"129.132.2.21")==0) check="selected";
    else check=" ";
    printf("                <option value=\"129.132.2.21\" %s>Swiss FIT (129.132.2.21)</option>\n",check);
    if (strcmp(options.ntpip,"193.49.205.17")==0) check="selected";
    else check=" ";
    printf("                <option value=\"193.49.205.17\" %s>Institut Curie Paris (193.49.205.17)</option>\n",check);
    printf("            </select></p>\n");
    printf("        <h3 class=\"content_text\">IP Forwarding</h3>\n");
    printf("            <p class=\"content_text\">IP Forwarding aktiviert\n");
    if (strcmp(options.ip_forwarding,"yes")==0) check="checked=\"checked\"";
    else check=" ";
    printf("            <input type=\"checkbox\" name=\"ip_forwarding\" value=\"yes\" %s></p>\n",check);
    printf("    </fieldset>\n");

    printf("    <fieldset>\n");
    printf("    <legend>Festplatteneinstellungen</legend>\n");
    printf("        <h3 class=\"content_text\">Festplattenparameter</h3>\n");
#warning "No checkbox for swap!"
    printf("            <p class=\"content_text\">DMA aktivieren\n");
    if (options.hd_dma==1) check="checked=\"checked\"";
    else check=" ";
    printf("            <input type=\"checkbox\" name=\"dma\" value=\"1\" %s><br>\n",check);
// FixME! the values should be 34 for DMA2, 66 for UDMA2, 68 for UDMA4 !! see http://www.thedumbterminal.co.uk/information/hdparm.shtml
    if (options.hd_transfer_mode==33) check="checked=\"checked\"";
    else check=" ";
    printf("            UDMA Modus 2<input type=\"radio\" name=\"udma\" value=\"33\" %s>\n",check);
    if (options.hd_transfer_mode==66) check="checked=\"checked\"";
    else check=" ";
    printf("            UDMA Modus 3<input type=\"radio\" name=\"udma\" value=\"66\" %s>\n",check);
    if (options.hd_transfer_mode==100) check="checked=\"checked\"";
    else check=" ";
    printf("            UDMA Modus 4<input type=\"radio\" name=\"udma\" value=\"100\" %s><br>\n",check);

    printf("            <p class=\"content_text\">Demaskierung anderer IRQs\n");
    if (options.hd_interrupt_umask==1) check="checked=\"checked\"";
    else check=" ";
    printf("            <input type=\"checkbox\" name=\"umaskirq\" value=\"1\" %s><br>\n",check);

    printf("            <p class=\"content_text\">Anzahl zu lesender Sektoren\n");
    printf("            <select name=\"sector_count\" size=\"1\">\n");
    if (options.hd_sector_count==2) check="selected";
    else check=" ";
    printf("                <option value=\"2\" %s>2</option>\n",check);
    if (options.hd_sector_count==4) check="selected";
    else check=" ";
    printf("                <option value=\"4\" %s>4</option>\n",check);
    if (options.hd_sector_count==8) check="selected";
    else check=" ";
    printf("                <option value=\"8\" %s>8</option>\n",check);
    if (options.hd_sector_count==16) check="selected";
    else check=" ";
    printf("                <option value=\"16\" %s>16</option>\n",check);
    printf("            </select></p>\n");

    printf("            <p class=\"content_text\">Benutze multi IO Support Modus\n");
    printf("            <select name=\"multi_io\" size=\"1\">\n");
    if (options.hd_32bit==0) check="selected";
    else check=" ";
    printf("                <option value=\"0\" %s>off (Modus 0)</option>\n",check);
    if (options.hd_32bit==2) check="selected";
    else check=" ";
    printf("                <option value=\"2\" %s>32bit+w/async (Modus 2)</option>\n",check);
    if (options.hd_32bit==3) check="selected";
    else check=" ";
    printf("                <option value=\"3\" %s>32bit+w/sync (Modus 3)</option>\n",check);
    printf("            </select></p>\n");

    printf("        <h4 class=\"content_text\">Spindown Zeit</h4></br><p class=\"content_text\">\n");
    if (options.hd_spindown==12) check="checked=\"checked\"";
    else check=" ";
    printf("            1min <input type=\"radio\" name=\"hd_to\" value=\"12\" %s>\n",check);
    if (options.hd_spindown==36) check="checked=\"checked\"";
    else check=" ";
    printf("            3min <input type=\"radio\" name=\"hd_to\" value=\"36\" %s>\n",check);
    if (options.hd_spindown==60) check="checked=\"checked\"";
    else check=" ";
    printf("            5min <input type=\"radio\" name=\"hd_to\" value=\"60\" %s>\n",check);
    if (options.hd_spindown==240) check="checked=\"checked\"";
    else check=" ";
    printf("            30min <input type=\"radio\" name=\"hd_to\" value=\"240\" %s>\n",check);
    if (options.hd_spindown==241) check="checked=\"checked\"";
    else check=" ";
    printf("            60min <input type=\"radio\" name=\"hd_to\" value=\"241\" %s>\n",check);
    if (options.hd_spindown==0) check="checked=\"checked\"";
    else check=" ";
    printf("            nie <input type=\"radio\" name=\"hd_to\" value=\"0\" %s>\n",check);
    printf("            <br>\n");
    printf("    </fieldset>\n");

    printf("    <input type=\"submit\" id=\"submit\" class=\"submit\" value=\"Speichern\" />\n");
    printf("    <input type=\"submit\" id=\"default\" class=\"submit\" value=\"setze Standard\" />\n");
    printf("    </form>\n");
    printf("</div><!-- end of #right -->\n");
#ifdef DEBUG
    snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] page glob_set end\n",__FUNCTION__,__LINE__);
    debug_info(debug_buf);
#endif
    page_content_end();
    page_footer();
}
