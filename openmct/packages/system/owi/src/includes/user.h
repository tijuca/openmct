#define	USER_FILE	"/etc/passwd"

int user_main(int argc, char **);
void user_list();
void user_detail(char *username);
void user_update(char *username, char *password, char *gecos, char *directory, char *loginshell);
