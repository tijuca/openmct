#ifdef LANG_GERMAN

/* user */

#define  USER_TABLE_DESCRIPTION              "Login"
#define  USER_TABLE_NEW_PASSWORD             "Password"
#define  USER_TABLE_GECOS                    "Full Name"
#define  USER_TABLE_ACTION                   "Action"
#define  USER_HEADLINE                       "Access: Users"
#define  USER_FILE_FAILED                    "Can't open user database"
#define  USER_NOT_FOUND                      "Can't find user"
#define  USER_BUTTON_NEW                     "New"
#define  USER_BUTTON_DELETE                  "Delete"
#define  USER_BUTTON_MODIFY                  "Edit"
#define  USER_BUTTON_UPDATE                  "Save"
#define  USER_BUTTON_ADD                     "Add"
#define  USER_NAV_DESCRIPTION                "User"
#define  USER_LOGIN_DESCRIPTION              "Unique login name of user."
#define  USER_GECOS_DESCRIPTION              "User full name."
#define  USER_PASSWORD_DESCRIPTION           "User password."
#define  USER_ALREADY_EXISTS                 "User already exists"
#define  USER_TABLE_SHELL                    "Full Shell"
#define  USER_SHELL_DESCRIPTION              "Give full shell to user"
#define  USER_LOGIN_INVALID                  "Invalid syntax for username"
#define  USER_GECOS_INVALID                  "Invalid syntax for description"

/* group */

#define  GROUP_TABLE_DESCRIPTION             "Group"
#define  GROUP_TABLE_MEMBERS                 "Members"
#define  GROUP_TABLE_ACTION                  "Action"
#define  GROUP_BUTTON_DELETE                 "Delete"
#define  GROUP_BUTTON_NEW                    "New"
#define  GROUP_BUTTON_MODIFY                 "Edit"
#define  GROUP_BUTTON_ADD                    "Add"
#define  GROUP_BUTTON_UPDATE                 "Save"
#define  GROUP_HEADLINE                      "Access: Groups"
#define  GROUP_NAV_DESCRIPTION               "Group"
#define  GROUP_FILE_FAILED                   "Can't open group database"
#define  GROUP_NOT_FOUND                     "Can't find group"
#define  GROUP_DESCRIPTION                   "Unique group name."
#define  GROUP_MEMBERS_DESCRIPTION           "Comma separated member list."
#define  GROUP_INVALID                       "Invalid syntax for groupname"
#define  GROUP_MEMBERS_INVALID               "Invalid syntax for members"
#define  GROUP_ALREADY_EXISTS                "Group already exists"

/* sysinfo */
#define  SYSINFO_HEADLINE                    "OpenMCT"
#define  SYSINFO_HEADLINE_BOX                "Systeminformation"
#define  SYSINFO_NAV_SYSTEM                  "System"
#define  SYSINFO_NAV_DESCRIPTION             "DESC"
#define  SYSINFO_PROCESS_HEADLINE            "Status: Processes"
#define  SYSTEM_NAV_DESCRIPTION              "System"
#define  STATUS_NAV_DESCRIPTION              "Status"
#define  PROCESS_NAV_DESCRIPTION             "Process"

/* nfs */
#define  NFS_TABLE_DESCRIPTION               "Share"
#define  NFS_TABLE_OPTIONS		     "Properties"
#define  NFS_TABLE_ACTION                    "Action"
#define  NFS_HEADLINE                        "Services: NFS"
#define  NFS_FILE_FAILED                     "Can't open NFS database"
#define  NFS_NOT_FOUND                       "Can't find NFS share"
#define  NFS_BUTTON_DELETE                   "Delete"
#define  NFS_BUTTON_UPDATE                   "Save"
#define  NFS_BUTTON_MODIFY                   "Edit"
#define  NFS_BUTTON_NEW                      "New"
#define  NFS_BUTTON_ADD                      "Add"
#define  NFS_NAV_DESCRIPTION                 "NFS"
#define  NFS_DESCRIPTION                     "Unique share name."
#define  NFS_OPTIONS_DESCRIPTION             "Share properties."
#define  NFS_INVALID                         "Invalid syntax for share"
#define  NFS_OPTIONS_INVALID                 "Invalid syntax for share options"
#define  NFS_ALREADY_EXISTS                  "Share already exists"

/* ftp */
#define  FTP_HEADLINE                        "Services: FTP"
#define  FTP_FILE_FAILED                     "Can't open FTP configuration"

#define  FTP_TABLE_LISTEN                    "Listen"
#define  FTP_TABLE_LISTEN_YES                "Yes"
#define  FTP_TABLE_LISTEN_NO                 "No"

#define  FTP_BUTTON_UPDATE                   "Save"

#define  FTP_INI_LISTEN	                     "Daemon Mode"
#define  FTP_INI_ANONYMOUS_ENABLE            "Enable anonymous logins"
#define  FTP_INI_LOCAL_ENABLE                "Enable local users"
#define  FTP_INI_WRITE_ENABLE                "Enable write access"
#define  FTP_INI_LOCAL_UMASK                 "Local umask"
#define  FTP_INI_DIRMESSAGE_ENABLED          "Directory message"
#define  FTP_INI_XFERLOG_ENABLE              "Log up/download"
#define  FTP_INI_CONNECT_FROM_PORT_20        "PORT Port 20"
#define  FTP_INI_SECURE_CHROOT_DIR           "Secure directory (chroot/jail)"
#define  FTP_INI_FTPD_BANNER                 "Banner"
#define  FTP_NAV_DESCRIPTION                 "FTP"

/* shell */
#define  SHELL_HEADLINE                      "Remote Console"
#define  SHELL_DESCRIPTION                   "Dieser Zugang bietet Ihnen die M&ouml;glichkeit lokale Befehle einzugeben und deren Ausgabe einzusehen. Bitte beachten Sie, dass Programme, die eine Eingabe von der Tastatur erfordern, aufgrund des Aufbau dieser Seite nicht funktionieren (zB top, vi..). Diese Console soll als Notfalll&ouml;sung dienen bzw zur schnellen kurzen Eingabe von Befehlen."
#define  SHELL_TABLE_COMMAND                 "Befehl"
#define  SHELL_TABLE_SUBMIT                  "Ausf&uuml;hren"
#define  SHELL_NAV_DESCRIPTION               "Konsole"

/* interface */

#define  INTERFACE_TABLE_DESCRIPTION              "Interface"
#define  INTERFACE_TABLE_IPADDRESS                "IP Adresse"
#define  INTERFACE_TABLE_NETMASK                  "Netzmaske"
#define  INTERFACE_TABLE_BROADCAST                "Broadcast"
#define  INTERFACE_TABLE_GATEWAY                  "Default Route"
#define  INTERFACE_TABLE_ACTION                   "Aktion"
#define  INTERFACE_HEADLINE                       "Netzwerkschnittstellen"
#define  INTERFACE_DESCRIPTION                    "Die nachfolgende Liste f&uuml;hrt alle Ihre Netzwerkschnittstellen auf. Hier k&ouml;nnen Sie alle Eigenschaften der einzelnen Schnittstellen im &Uuml;berblick einsehen. F&uuml;r mehr Informationen zu einer Schnittstelle verwenden Sie bitte den <i>Bearbeiten</i> Button in der entsprechenden Zeile."
#define  INTERFACE_DETAIL                         "Sie befinden Sich nun im Detailbereich f&uuml;r eine Netzwerkschnittstelle. Hier k&ouml;nnen Sie sowohl die IP Adresse und Netzmaske, als auch ein Default Gateway (falls vorhanden) eintragen. ACHTUNG: Die Einstellungen werden erst beim n&auml;chsten Neustart &uuml;bernommen, um eine Kontrolle zu ermoeglichen."
#define  INTERFACE_NEW                            "In diesem Bereich k&ouml;nnen Sie eine neue Netzwerkschnittstelle anlegen. Bitte geben Sie dazu alle Felder an um sicherzustellen, dass die Schnittstelle dann ordnungsgem&auml;&szlig; funktioniert. Sobald alle Daten eingegeben worden sind kann die Aktion mit dem Klicken auf den <i>Neu</i> Button abgeschlossen werden."
#define  INTERFACE_FILE_FAILED                    "Netzwerkschnittstellendatenbank konnte nicht ge&ouml;ffnet werden"
#define  INTERFACE_NOT_FOUND                      "Netzwerkschnittstelle konnte nicht gefunden werden"
#define  INTERFACE_BUTTON_NEW                     "Neue Netzwerkschnittstelle"
#define  INTERFACE_BUTTON_DELETE                  "Entfernen"
#define  INTERFACE_BUTTON_MODIFY                  "Bearbeiten"
#define  INTERFACE_BUTTON_UPDATE                  "Speichern"
#define  INTERFACE_BUTTON_ADD                     "Hinzuf&uuml;gen"
#define  INTERFACE_INI_ADDRESS                    "Adresse"
#define  INTERFACE_INI_NETMASK                    "Netzmaske"
#define  INTERFACE_INI_GATEWAY                    "Default Gateway"
#define  INTERFACE_INI_PROVIDER                   "Provider"
#define  INTERFACE_NAV_DESCRIPTION                "Netzwerk"

#define  ACCESS_NAV_DESCRIPTION                   "System"

#define  SERVICES_NAV_DESCRIPTION                 "Services"

#endif
