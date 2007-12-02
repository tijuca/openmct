struct modules_t {
   char *name;
   int (*main)(int, char **);
   char *description;
   int  level;
} modules[] = {
   { "services", NULL, SERVICES_NAV_DESCRIPTION, 0 },
   { "ftp", ftp_main, FTP_NAV_DESCRIPTION, 1 },
   { "nfs", nfs_main, NFS_NAV_DESCRIPTION, 1 },
   { "smb", smb_main, SMB_NAV_DESCRIPTION, 1 },
   { "interfaces", NULL, INTERFACES_NAV_DESCRIPTION, 0 },
   { "lan", lan_main, INTERFACE_LAN_NAV_DESCRIPTION, 1 },
   { "status", NULL, STATUS_NAV_DESCRIPTION, 0 },
   { "process", process_main, PROCESS_NAV_DESCRIPTION, 1 },
   { "system", NULL, SYSTEM_NAV_DESCRIPTION, 0 },
   { "harddisk", harddisk_main, HARDDISK_NAV_DESCRIPTION, 1 },
   { "user", user_main, USER_NAV_DESCRIPTION, 1 },
   { "group", group_main, GROUP_NAV_DESCRIPTION, 1 },
   { "shell", shell_main, SHELL_NAV_DESCRIPTION, 1 },
   { NULL, NULL, NULL, 0 }
};
