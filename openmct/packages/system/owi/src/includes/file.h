#define  FILE_MAXLINE                1024
#define  FILE_LINE_SET               1
#define  FILE_LINE_DEL               2
#define  FILE_LINE_ADD               3

#define  FILE_OPEN_OK(x)             (x)->line

#define  FILE_FLAG_DONT_SKIP_COMMENT 1

#define  FILE_ITERATOR_START         1
#define  FILE_ITERATOR_NEXT          2

#define  file_length(file) \
         array_length(file->line)

#define  file_line(file, index) \
         array_value(file->line, index)

#define  file_iterator_next(file, index) \
         file_iterator(file, FILE_ITERATOR_NEXT, index)

#define  file_iterator_start(file, index) \
         file_iterator(file, FILE_ITERATOR_START, index)

struct file_t {
   struct string_t *name;
   struct string_t *separator;
   struct array_t  *line;
   unsigned int    line_current;
   struct array_t  *current;
   unsigned int    flags;
};

void file_open(struct file_t *f);
void file_read(struct file_t *f, FILE *fp);
void file_free(struct file_t *f);
void file_save(struct file_t *f);
void file_data_update(struct file_t *f);
void file_value_set(struct file_t *f, unsigned int name_index, struct string_t *name, unsigned int value_index, struct string_t *value);
struct string_t *file_value_get(struct file_t *f, unsigned int name_index, struct string_t *name, unsigned int value_index);
void file_value_del(struct file_t *f, int name_index, struct string_t *name);
void file_action(struct file_t *f, int mode, char *format, ...);
void proc_open(struct file_t *f, char *command);
void file_iterator(struct file_t *f, unsigned int mode, unsigned int *index);
