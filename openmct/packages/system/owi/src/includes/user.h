#define	USER_FILE	"/etc/passwd"
#define USER_SEPARATOR  ":"

#define USER_SHELL_FALSE   "/bin/false"
#define USER_SHELL_DEFAULT "/bin/sh"

int user_main(int argc, char **arv);
void user_add(struct file_t *f);
