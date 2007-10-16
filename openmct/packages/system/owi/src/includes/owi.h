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

void owi_data_detail(struct file_data_t *ini);
void owi_data_list(struct file_data_t *ini, char **entry);
void owi_table_header(struct file_data_t *ini);
