#ifdef LANG_GERMAN

/* user */

#define  USER_HEADLINE                       "Access: Users"
#define  USER_FILE_FAILED                    "Can't open user database"
#define  USER_NOT_FOUND                      "Can't find user"
#define  USER_NAV_DESCRIPTION                "User"
#define  USER_ALREADY_EXISTS                 "User already exists"

#define  USER_NAME_LOGIN                     "Login"
#define  USER_NAME_PASSWORD                  "Password"
#define  USER_NAME_GECOS                     "Full Name"
#define  USER_NAME_SHELL                     "Full Shell"

#define  USER_DESCRIPTION_LOGIN              "Unique login name of user."
#define  USER_DESCRIPTION_PASSWORD           "User password."
#define  USER_DESCRIPTION_GECOS              "User full name."
#define  USER_DESCRIPTION_SHELL              "Give full shell to user."

#define  USER_FILE_UPDATE                    "User has been changed"
#define  USER_FILE_ERROR                     "User has not been changed"

/* group */

#define  GROUP_HEADLINE                       "Access: Groups"
#define  GROUP_FILE_FAILED                    "Can't open group database"
#define  GROUP_NOT_FOUND                      "Can't find group"
#define  GROUP_NAV_DESCRIPTION                "Group"
#define  GROUP_ALREADY_EXISTS                 "Group already exists"

#define  GROUP_NAME_LOGIN                     "Group"
#define  GROUP_NAME_MEMBERS                   "Members"

#define  GROUP_DESCRIPTION_LOGIN              "Unique name of group."
#define  GROUP_DESCRIPTION_MEMBERS            "Group members."

#define  GROUP_FILE_UPDATE                    "Group has been changed"
#define  GROUP_FILE_ERROR                     "Group has not been changed"

/* sysinfo */
#define  SYSINFO_HEADLINE                    "OpenMCT"
#define  SYSINFO_HEADLINE_BOX                "Systeminformation"
#define  SYSINFO_NAV_SYSTEM                  "System"
#define  SYSINFO_NAV_DESCRIPTION             "DESC"
#define  SYSTEM_NAV_DESCRIPTION              "System"
#define  STATUS_NAV_DESCRIPTION              "Status"
#define  PROCESS_NAV_DESCRIPTION             "Process"

/* process */
#define  PROCESS_HEADLINE                    "Status: Processes"

/* nfs */

#define  NFS_HEADLINE                        "Services: NFS"
#define  NFS_FILE_FAILED                     "Can't open NFS database"
#define  NFS_NOT_FOUND                       "Can't find NFS share"
#define  NFS_NAV_DESCRIPTION                 "NFS"
#define  NFS_ALREADY_EXISTS                  "Directory already exists"

#define  NFS_NAME_DIRECTORY                  "Directory"
#define  NFS_NAME_SOURCE                     "Source"
#define  NFS_NAME_OPTIONS                    "Options"
#define  NFS_DESCRIPTION_DIRECTORY           "Unique directory name."
#define  NFS_DESCRIPTION_SOURCE              "Network/Host that is authorized to access to NFS share."
#define  NFS_DESCRIPTION_OPTIONS             "Properties for share."

#define  NFS_FILE_UPDATE                     "Share has been changed"
#define  NFS_FILE_ERROR                      "Share has not been changed"

/* ftp */
#define  FTP_HEADLINE                        "Services: FTP"
#define  FTP_FILE_FAILED                     "Can't open FTP configuration"
#define  FTP_FILE_UPDATE                     "FTP configuration has been changed"
#define  FTP_FILE_ERROR                      "FTP configuration has not been changed"

#define  FTP_NAME_LISTEN_PORT	             "TCP port"
#define  FTP_NAME_MAX_CLIENTS                "Number of clients"
#define  FTP_NAME_MAX_PER_IP                 "Max conn per ip"
#define  FTP_NAME_IDLE_SESSION_TIMEOUT       "Timeout"
#define  FTP_NAME_ANONYMOUS_ENABLE           "Anonymous login"
#define  FTP_NAME_LOCAL_ENABLE               "Local User"
#define  FTP_NAME_BANNER                     "Banner"
#define  FTP_NAME_FILE_OPEN_MODE             "Create mask"
#define  FTP_NAME_PASV_PROMISCUOUS           "FXP"

#define  FTP_DESCRIPTION_MAX_CLIENTS         "Maximum number of simultaneous clients."
#define  FTP_DESCRIPTION_MAX_PER_IP          "Maximum connection per IP address."
#define  FTP_DESCRIPTION_IDLE_SESSION_TIMEOUT "Maximum idle time in seconds."
#define  FTP_DESCRIPTION_ANONYMOUS_ENABLE    "Enable anonymous login."
#define  FTP_DESCRIPTION_LOCAL_ENABLE        "Enable local user."
#define  FTP_DESCRIPTION_BANNER              "Greeting banner displayed by FTP when a connection first comes in."
#define  FTP_DESCRIPTION_FILE_OPEN_MODE      "Use this option to override the file creation mask (077 by default)."
#define  FTP_DESCRIPTION_PASV_PROMISCUOUS    "Enable FXP protocol. FXP allows transfers between two remote servers without any file data going to the client asking for the transfer (insecure!)."

#define  FTP_NAV_DESCRIPTION                 "FTP"

/* smb */
#define  SMB_HEADLINE                        "Services: SMB"

#define  SMB_NAME_WORKGROUP                  "Workgroup"
#define  SMB_NAME_SERVER_STRING              "Server String"

#define  SMB_DESCRIPTION_WORKGROUP           "Workgroup."
#define  SMB_DESCRIPTION_SERVER_STRING       "Server description that will be displayed in network settings"

#define  SMB_FILE_UPDATE                     "SMB configuration has been changed"
#define  SMB_FILE_ERROR                      "SMB configuration has not been changed"

#define  SMB_NAV_DESCRIPTION                 "SMB"

/* rc */
#define  RC_NAME_START_FTPD                  "Enable FTP"
#define  RC_NAME_START_NFSD                  "NFS"
#define  RC_NAME_START_SMBD                  "Enable SMB"

#define  RC_DESCRIPTION_START_FTPD           "Start FTP daemon during boot sequence"
#define  RC_DESCRIPTION_START_SMBD           "Start SMB daemon during boot sequence"
#define  RC_DESCRIPTION_START_NFSD           "Enable NFS"

#define  RC_FILE_UPDATE                      "Settings has been changed -> Reboot required"
#define  RC_FILE_ERROR                       "Settings has not been changed"

/* harddisk */
#define  HARDDISK_HEADLINE                   "System: Harddisk"

#define  HARDDISK_NAV_DESCRIPTION            "Harddisk"

#define  HARDDISK_NAME_DEVICE                "Device"
#define  HARDDISK_NAME_DMA                   "Enable DMA"
#define  HARDDISK_NAME_INTERRUPT_UNMASK      "Enable interrupt unmask"
#define  HARDDISK_NAME_SECTOR_COUNT          "Sector count"
#define  HARDDISK_NAME_32BIT                 "Enable 32Bit"
#define  HARDDISK_NAME_SPINDOWN_TIMEOUT      "Spindown Timeout"

#define  HARDDISK_FILE_UPDATE                "Harddisk settings has been changed -> Reboot required"
#define  HARDDISK_FILE_ERROR                 "Harddisk settings has not been changed"

/* shell */
#define  SHELL_HEADLINE                      "Shell"
#define  SHELL_TABLE_COMMAND                 "Command"
#define  SHELL_TABLE_SUBMIT                  "Execute"
#define  SHELL_NAV_DESCRIPTION               "Shell"

/* interface */
#define  INTERFACE_FILE_FAILED               "Can't open network file"
#define  INTERFACE_FILE_UPDATE               "Interface configuration has been changed"
#define  INTERFACE_FILE_ERROR                "Interface configuration has not been changed"

#define  INTERFACE_NAME                      "Interface"
#define  INTERFACE_NAME_ADDRESS              "IP Address"
#define  INTERFACE_NAME_NETMASK              "Netmask"
#define  INTERFACE_NAME_BROADCAST            "Broadcast"
#define  INTERFACE_NAME_GATEWAY              "Default Route"

#define  INTERFACE_DESCRIPTION_ADDRESS       "IP Address of network interface."
#define  INTERFACE_DESCRIPTION_NETMASK       "Netmask of network interface."
#define  INTERFACE_DESCRIPTION_BROADCAST     "Broadcast of network interface."
#define  INTERFACE_DESCRIPTION_GATEWAY       "Gateway of network interface."

/* interface: LAN */
#define  INTERFACE_LAN_HEADLINE              "Interfaces: LAN"
#define  INTERFACE_LAN_NOT_FOUND             "Can't find LAN interface"
#define  INTERFACE_LAN_NAV_DESCRIPTION       "LAN"

#define  SYSTEM_NAV_DESCRIPTION              "System"
#define  SERVICES_NAV_DESCRIPTION            "Services"
#define  INTERFACES_NAV_DESCRIPTION          "Interfaces"

#define  OWI_BUTTON_ADD                      "Add"
#define  OWI_BUTTON_NEW                      "New"
#define  OWI_BUTTON_DETAIL                   "Details"
#define  OWI_BUTTON_UPDATE                   "Save"
#define  OWI_BUTTON_DELETE                   "Delete"
#define  OWI_BUTTON_LIST                     "List"

#define  OWI_SYNTAX_INVALID                  "Invalid syntax"
#define  OWI_ERROR_INVALID                   "Error"

#define  OWI_OBJECT_NOT_FOUND                "Object not found"
#define  OWI_OBJECT_FOUND                    "Object found"

#endif
