#define  FILE_MAXLINE                1024
#define  FILE_LINE_SET               1
#define  FILE_LINE_DEL               2
#define  FILE_LINE_ADD               3

struct file_line_t {
   char *data;
   char **current;
};

struct file_t {
   char               *name;
   char               *separator;
   int                line_count;
   int                line_current;
   struct file_line_t **line;
   char               *command_update;
};

void file_open(struct file_t *f, char *filename);
void file_read(struct file_t *f, FILE *fp);
void file_free(struct file_t *f);
void file_save(struct file_t *f);
void file_data_update(struct file_t *f);
void file_value_set(struct file_t *f, int name_index, char *name, int value_index, char *value);
char *file_value_get(struct file_t *f, int name_index, char *name, int value_index);
void file_value_del(struct file_t *f, int name_index, char *name);
char *file_value_get(struct file_t *f, int name_index, char *name, int value_index);
void file_action(struct file_t *f, int mode, char *format, ...);
void proc_open(struct file_t *f, char *command);
