#define	NFS_FILE	"/etc/exports"

#define NFS_VALID "^[a-z0-9_/]{1,64}$"
#define NFS_OPTIONS_VALID "^[a-zA-Z0-9_,()]*$"

int nfs_main(int argc, char **arv);
void nfs_list();
void nfs_detail(char *nfsname);
void nfs_update(char *nfsname);
void nfs_delete(char *nfsname);
void nfs_add(char *nfsname);
void nfs_new();
