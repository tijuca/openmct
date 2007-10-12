#define GROUP_FILE       "/etc/group"

#define GROUP_VALID "^[a-z0-9_]{1,8}$"
#define GROUP_MEMBERS_VALID "^[a-zA-Z0-9_,]*$"

int group_main(int argc, char **);
void group_list();
void group_detail(char *groupname);
void group_update(char *groupname, char *members);
void group_delete(char *groupname);
void group_new();
void group_add();
