#include <stdio.h>
#include "user.h"
#include "group.h"
#include "sysinfo.h"

struct modules_t {
   char *name;
   int (*main)();
} modules[] = {
   { "user.cgi", user_main },
   { "group.cgi", group_main },
   { "sysinfo.cgi", sysinfo_main },
   { NULL, NULL }
};
