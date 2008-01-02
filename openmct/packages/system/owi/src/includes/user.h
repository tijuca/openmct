#define	USER_FILE	    "/etc/passwd"
#define USER_SEPARATOR      ":"

#define USER_UID_STANDARD   "100"
#define USER_GID_STANDARD   "100"

#define USER_SHELL_FALSE    "/bin/false"
#define USER_SHELL_DEFAULT  "/bin/sh"

#define USER_HOMEDIRECTORY_PREFIX	"/home/"

#define USER_INDEX_UID      2
#define USER_INDEX_HOMEDIRECTORY	5

int user_main(struct owi_t *owi);
struct string_t *user_callback(struct owi_t *owi, struct string_t *command);
