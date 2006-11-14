#define	NFS_FILE	"/etc/exports"

int nfs_main(int argc, char **arv);
void nfs_list();
void nfs_detail(char *nfsname);
void nfs_update(char *nfsname, char *options);
void nfs_add(char *nfsname, char *options);
