#include <stdio.h>
#include "user.h"
#include "group.h"
#include "sysinfo.h"
#include "nfs.h"
#include "ftp.h"
#include "shell.h"
#include "interface.h"

struct modules_t {
   char *name;
   int (*main)(int, char **);
} modules[] = {
   { "user.cgi", user_main },
   { "group.cgi", group_main },
   { "sysinfo.cgi", sysinfo_main },
   { "nfs.cgi", nfs_main },
   { "ftp.cgi", ftp_main },
   { "shell.cgi", shell_main },
   { "interface.cgi", interface_main },
   { NULL, NULL }
};
