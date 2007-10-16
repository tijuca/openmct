#define	FTP_FILE	"/etc/vsftpd.conf"
#define FTP_RESTART     "/bin/killall -HUP vsftpd"

int ftp_main(int argc, char **arv);
void ftp_list();
void ftp_update();
