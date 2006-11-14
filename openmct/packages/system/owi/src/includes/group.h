#define GROUP_FILE       "/etc/group"

int group_main(int argc, char **);
void group_list();
void group_detail(char *username);
void group_update(char *groupname, char *members);
