#define	INTERFACE_FILE	"/etc/network/interfaces"

int interface_main(int argc, char **arv);
void interface_list();
void interface_detail(char *interfacename);
void interface_update(char *interfacename);
void interface_delete(char *interfacename);
void interface_new();
void interface_add();
