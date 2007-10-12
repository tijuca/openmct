#include <stdio.h>
#include "user.h"
#include "group.h"
#include "sysinfo.h"
#include "nfs.h"
#include "ftp.h"
#include "shell.h"
#include "interface.h"
#include "language.h"
#include "template.h"

struct modules_t {
   char *name;
   int (*main)(int, char **);
   char *description;
   int  level;
} modules[] = {
/*
   { "system", NULL, SYSINFO_NAV_SYSTEM, 0 },
   { "sysinfo", sysinfo_main, SYSINFO_NAV_DESCRIPTION, 1},
   { "interface", interface_main, INTERFACE_NAV_DESCRIPTION, 1 },
   { "nfs", nfs_main, NFS_NAV_DESCRIPTION, 1 },
   { "ftp", ftp_main, FTP_NAV_DESCRIPTION, 1 },
   { "user", user_main, USER_NAV_DESCRIPTION, 1 },
   { "group", group_main, GROUP_NAV_DESCRIPTION, 1 },
#ifdef SECURITY
   { "shell", shell_main, SHELL_NAV_DESCRIPTION, 1 },
#endif   */
   { "services", NULL, SERVICES_NAV_DESCRIPTION, 0 },
   { "ftp", ftp_main, FTP_NAV_DESCRIPTION, 1 },
   { "nfs", nfs_main, NFS_NAV_DESCRIPTION, 1 },
   { "status", NULL, STATUS_NAV_DESCRIPTION, 0 },
   { "system", sysinfo_main, SYSTEM_NAV_DESCRIPTION, 1 },
   { "system&amp;command=process", sysinfo_main, PROCESS_NAV_DESCRIPTION, 1 },
   { "access", NULL, ACCESS_NAV_DESCRIPTION, 0 },
   { "user", user_main, USER_NAV_DESCRIPTION, 1 },
   { "group", group_main, GROUP_NAV_DESCRIPTION, 1 },
   { NULL, NULL, NULL, 0 }
};
