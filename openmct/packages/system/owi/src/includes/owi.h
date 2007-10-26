#define OWI_DETAIL	1
#define OWI_LIST	0

void owi_header(char *title);
void owi_footer();
void owi_headline(int size, char *headline);
void owi_request();
void owi_outside_open(int mode);
void owi_outside_close(int mode, char *value);
void owi_error(char *variable);
void owi_box_error();
void owi_box_info();

void owi_data_detail(struct file_t *f, int flags);
void owi_data_list(struct file_t *f);
void owi_table_header(struct file_t *f);

void owi_list(struct file_t *f);

void owi_new(struct file_t *f);
void owi_add(struct file_t *f);

void owi_detail(struct file_t *f);
void owi_detail_id(struct file_t *f, char *id);

void owi_update(struct file_t *f, char *info, char *error);
void owi_update_id(struct file_t *f, char *id, char *info, char *error);

void owi_delete_id(struct file_t *f, char *id);
