#ifdef LANG_GERMAN

/* user */

#define  USER_TABLE_DESCRIPTION              "Kennung"
#define  USER_TABLE_NEW_PASSWORD             "Neues Passwort"
#define  USER_TABLE_NEW_PASSWORD_CHECK       "Neues Passwort (Best&auml;tigung)"
#define  USER_TABLE_UID                      "User ID"
#define  USER_TABLE_GID                      "Gruppen ID"
#define  USER_TABLE_GECOS                    "Bemerkung"
#define  USER_TABLE_DIRECTORY                "Heimatverzeichnis"
#define  USER_TABLE_SHELL                    "Loginprogramm"
#define  USER_TABLE_ACTION                   "Aktion"
#define  USER_HEADLINE                       "Benutzerverwaltung"
#define  USER_DESCRIPTION                    "Die nachfolgende Liste f&uuml;hrt Ihre System Kennungen an. Hier k&ouml;nnen Sie alle Kennungen (auch das Administrator Konto) einsehen und &Auml;nderungen durchf&uuml;hren. F&uuml;r mehr Informationen zu einer Kennung verwenden Sie bitte den <i>Bearbeiten</i> Button in der entsprechenden Zeile."
#define  USER_DETAIL                         "Sie befinden Sich nun im Detailbereich f&uuml;r eine Kennung. Hier k&ouml;nnen Sie sowohl das Kennwort als auch das Heimatverzeichnis sowies diverse andere Einstellungen (Beschreibung, Loginprogramm) aktualisieren. Die Einstellungen werden mit dem Bet&auml;tigen des <i>Speichern</i> Buttons sofort &uuml;bernommen."
#define  USER_NEW                            "In diesem Bereich k&ouml;nnen Sie eine neue System Kennung erstellen. Bitte geben Sie dazu alle Felder an um sicherzustellen, dass die Kennung dann ordnungsgem&auml;&szlig; funktioniert. Sobald alle Daten eingegeben worden sind kann die Aktion mit dem Klicken auf den <i>Neu</i> Button abgeschlossen werden."
#define  USER_FILE_FAILED                    "Benutzerdatenbank konnte nicht ge&ouml;ffnet werden"
#define  USER_NOT_FOUND                      "Kennung konnte nicht gefunden werden"

#define  USER_BUTTON_NEW                     "Neu"
#define  USER_BUTTON_DELETE                  "Entfernen"
#define  USER_BUTTON_MODIFY                  "Bearbeiten"
#define  USER_BUTTON_UPDATE                  "Speichern"
#define  USER_BUTTON_ADD                     "Hinzuf&uuml;gen"

/* group */

#define  GROUP_TABLE_DESCRIPTION             "Gruppe"
#define  GROUP_TABLE_NEW_PASSWORD            "Neues Passwort"
#define  GROUP_TABLE_NEW_PASSWORD_CHECK      "Neues Passwort (Best&auml;tigung)"
#define  GROUP_TABLE_GID                     "Gruppen ID"
#define  GROUP_TABLE_MEMBERS                 "Mitglieder"
#define  GROUP_BUTTON_DELETE                 "Entfernen"
#define  GROUP_BUTTON_MODIFY                 "Bearbeiten"
#define  GROUP_HEADLINE                      "Gruppenverwaltung"
#define  GROUP_FILE_FAILED                   "Gruppendatenbank konnte nicht ge&ouml;ffnet werden"

/* sysinfo */
#define  SYSINFO_HEADLINE                    "Systeminformation"
#define  SYSINFO_DESCRIPTION                 "Anbei finden Sie aktuelle Informationen zum laufenden System"
#define  SYSINFO_NETWORK_DEVICES             "Netzwerkger&auml;te"
#define  SYSINFO_NETWORK_DEVICES_DESCRIPTION "Folgende Netzwerkkomponenten sind derzeit auf dem System konfiguriert"
#define  SYSINFO_FILESYSTEMS                 "Dateisysteme"
#define  SYSINFO_FILESYSTEMS_DESCRIPTION     "Anbei finden Sie alle aktuell verwendeten Dateisysteme"

/* nfs */
#define  NFS_TABLE_DESCRIPTION               "Freigabe"
#define  NFS_TABLE_OPTIONS		     "Eigenschaften"
#define  NFS_TABLE_ACTION                    "Aktion"
#define  NFS_HEADLINE                        "NFS (Network File System) Verwaltung"
#define  NFS_DESCRIPTION                     "Die nachfolgende Liste f&uuml;hrt Ihre NFS Freigaben an inkl. Eigenschaften wie freigeschaltete IP Adressen, Schreibzugriff etc. Bitte beachten Sie, dass diese Freigaben nur f&uuml;r den Dienst NFS gelten. Die Freigaben f&uuml;r Windows sind von diesen Einstellungen nicht betroffen."
#define  NFS_FILE_FAILED                     "NFS Datenbank konnte nicht ge&ouml;ffnet werden"

#define  NFS_BUTTON_DELETE                   "Entfernen"
#define  NFS_BUTTON_MODIFY                   "Bearbeiten"

/* ftp */
#define  FTP_HEADLINE                        "FTP (File Transfer Protocol) Verwaltung"
#define  FTP_DESCRIPTION                     "Diese Einstellungsbereich erlaubt Ihnen die Einstellungen Ihres FTP Servers zu &auml;ndern. Nachdem die Einstellungen mit dem <i>Speichern</i> Button best&auml;tigt werden, wird die Konfiguration des FTP Servers automatisch neu geladen."
#define  FTP_FILE_FAILED                     "FTP (File Transfer Protocol) Konfiguration konnte nicht ge&ouml;ffnet werden"

#define  FTP_TABLE_LISTEN                    "Listen"
#define  FTP_TABLE_LISTEN_YES                "Ja"
#define  FTP_TABLE_LISTEN_NO                 "Nein"

#define  FTP_BUTTON_UPDATE                  "Speichern"

#define  FTP_INI_LISTEN	                     "Daemon Modus"
#define  FTP_INI_ANONYMOUS_ENABLE            "Erlaube anonymous Logins"
#define  FTP_INI_LOCAL_ENABLE                "Lokale Accounts fuer FTP Zugriff erlauben"
#define  FTP_INI_WRITE_ENABLE                "Schreibzugriff"
#define  FTP_INI_LOCAL_UMASK                 "Lokale Umask"
#define  FTP_INI_DIRMESSAGE_ENABLED          "Verzeichnis Nachrichten einschalten"
#define  FTP_INI_XFERLOG_ENABLE              "Up/Download mitprotokollieren"
#define  FTP_INI_CONNECT_FROM_PORT_20        "PORT Befehlen sollen Port 20 verwenden"
#define  FTP_INI_SECURE_CHROOT_DIR           "Sicheres Verzeichnis (chroot/jail)"
#define  FTP_INI_FTPD_BANNER                 "Begr&uuml;&szlig;ungstext"

/* shell */
#define  SHELL_HEADLINE                      "Remote Console"
#define  SHELL_DESCRIPTION                   "Dieser Zugang bietet Ihnen die M&ouml;glichkeit lokale Befehle einzugeben und deren Ausgabe einzusehen. Bitte beachten Sie, dass Programme, die eine Eingabe von der Tastatur erfordern, aufgrund des Aufbau dieser Seite nicht funktionieren (zB top, vi..). Diese Console soll als Notfalll&ouml;sung dienen bzw zur schnellen kurzen Eingabe von Befehlen."
#define  SHELL_TABLE_COMMAND                 "Befehl"
#define  SHELL_TABLE_SUBMIT                  "Ausf&uuml;hren"

#endif
