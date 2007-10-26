#define	FTP_FILE	"/etc/vsftpd.conf"
#define FTP_RESTART     "/bin/killall -HUP vsftpd"
#define FTP_SEPARATOR   "="

int ftp_main(int argc, char **arv);
