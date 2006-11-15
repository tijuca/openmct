#define	USER_FILE	"/etc/passwd"

int user_main(int argc, char **arv);
void user_list();
void user_detail(char *username);
void user_update(char *username);
void user_delete(char *username);
void user_new();
void user_add();
