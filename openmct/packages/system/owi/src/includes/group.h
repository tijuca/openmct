#define GROUP_FILE       "/etc/group"

int group_main(int argc, char **);
void group_list();
void group_detail(char *groupname);
void group_update(char *groupname, char *members);
void group_delete(char *groupname);
void group_new();
void group_add();
