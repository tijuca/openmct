#define OWI_FLAG_CONFIG		1
#define OWI_FLAG_ACTION         2
#define OWI_FLAG_ROW            4
#define OWI_FLAG_HIDE_NEW	8

struct owi_t {
   struct data_t *data;
   struct file_t *file;
   struct data_t *data_init;
   struct file_t *file_init;
   char   *headline;
   char   **button;
   int    flags;
};

void owi_main(struct owi_t *owi);
void owi_header(struct owi_t *owi);
void owi_footer(struct owi_t *owi, char *button, ...);
void owi_table_header(struct owi_t *owi);
void owi_request();
void owi_data_display(struct data_t *data, char *current, int flags);

void owi_detail(struct owi_t *owi);
void owi_list(struct owi_t *owi);
