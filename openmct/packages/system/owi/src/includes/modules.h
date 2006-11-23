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
   char *style;
} modules[] = {
   { "sysinfo.cgi", sysinfo_main, SYSINFO_NAV_DESCRIPTION, SYSINFO_NAV_CLASS },
   { "interface.cgi", interface_main, INTERFACE_NAV_DESCRIPTION, INTERFACE_NAV_CLASS },
   { "nfs.cgi", nfs_main, NFS_NAV_DESCRIPTION, NFS_NAV_CLASS },
   { "ftp.cgi", ftp_main, FTP_NAV_DESCRIPTION, FTP_NAV_CLASS },
   { "user.cgi", user_main, USER_NAV_DESCRIPTION, USER_NAV_CLASS },
   { "group.cgi", group_main, GROUP_NAV_DESCRIPTION, GROUP_NAV_CLASS },
#ifdef SECURIT   
   { "shell.cgi", shell_main, SHELL_NAV_DESCRIPTION, SHELL_NAV_CLASS },
#endif   
   { NULL, NULL, NULL, NULL }
};
