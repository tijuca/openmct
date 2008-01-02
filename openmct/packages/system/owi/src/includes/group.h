#define	GROUP_FILE	"/etc/group"
#define GROUP_SEPARATOR ":"

#define GROUP_PASSWORD_STANDARD "x"

#define GROUP_GID_STANDARD   "100"

#define GROUP_INDEX_GID 2

int group_main(struct owi_t *owi);
struct string_t *group_callback(struct owi_t *owi, struct string_t *command);
