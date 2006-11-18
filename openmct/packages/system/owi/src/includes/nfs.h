#define	NFS_FILE	"/etc/exports"

int nfs_main(int argc, char **arv);
void nfs_list();
void nfs_detail(char *nfsname);
void nfs_update(char *nfsname);
void nfs_delete(char *nfsname);
void nfs_add(char *nfsname);
void nfs_new();
