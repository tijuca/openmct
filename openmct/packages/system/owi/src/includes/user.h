#define	USER_FILE	"/etc/passwd"

#define USER_SHELL_FALSE "/bin/false"
#define USER_SHELL_DEFAULT "/bin/sh"

#define USER_LOGIN_VALID "^[a-z0-9_]{1,8}$"
#define USER_GECOS_VALID "^[a-zA-Z0-9_ ]*$"

int user_main(int argc, char **arv);
void user_list();
void user_detail(char *username);
void user_update(char *username);
void user_delete(char *username);
void user_new();
void user_add();
