#define  FILE_MAXLINE      1024
#define  FILE_LINE_SET     1
#define  FILE_LINE_DEL     2
#define  FILE_LINE_ADD     3

#define  FILE_DATA_TYPE_TEXT      1
#define  FILE_DATA_TYPE_PASSWORD  2
#define  FILE_DATA_TYPE_CHECKBOX  3
#define  FILE_DATA_TYPE_SELECT    4
#define  FILE_DATA_TYPE_RADIO     5
#define  FILE_DATA_TYPE_TEXTAREA  6
#define  FILE_DATA_TYPE_INTERNAL  7

#define  FILE_DATA_FLAG_ADD          1
#define  FILE_DATA_FLAG_UPDATE       2
#define  FILE_DATA_FLAG_LIST         4
#define  FILE_DATA_FLAG_DONTFILL     8
#define  FILE_DATA_FLAG_ID          16
#define  FILE_DATA_FLAG_CRYPT       32
#define  FILE_DATA_FLAG_SKIP_EMPTY  64

#define  FILE_TYPE_LINE		1
#define  FILE_TYPE_SECTION      2
#define  FILE_TYPE_COMMAND	3

struct file_data_t {
   int  type;
   int  line;
   char *html;
   char *name;
   char *description;
   char *current;
   char *valid;
   char *directive;
   int  index;
   char *standard;
   int  flags;
};

struct file_t {
   char *name;
   int  type;
   char *separator;
   int  line_count;
   int  line_current;
   int  line_search;
   int  (*range_function)(char *pattern);
   struct file_data_t *fd;
   char **line;
};

void file_read(struct file_t *f, FILE *fp); 
void file_open(struct file_t *f, char *filename);
void file_data_free(struct file_t *f);
void file_free(struct file_t *f);
void file_data_read(struct file_t *f, char **entry);
void file_dump(struct file_t *f);
void file_save(struct file_t *f);
void file_data_update(struct file_t *f);
void file_action(struct file_t *f, int mode, char *format, ...);
int file_data_get_next(struct file_t *f);
int file_data_find(struct file_t *f, char *id);
char *file_data_write(struct file_t *f);
void proc_open(struct file_t *f, char *filename);
