#define	INTERFACE_FILE	"/etc/network/interfaces"
#define INTERFACE_LAN   "eth0"

int interface_main(int argc, char **arv);
int lan_main(int argc, char **arv);
void interface_list();
void interface_update();
void interface_section_get(char *interface);
