#define	NFS_FILE	"/etc/exports"
#define NFS_SEPARATOR	" \t"

int nfs_main(struct owi_t *owi);
struct string_t *nfs_callback(struct owi_t *owi, struct string_t *command);
