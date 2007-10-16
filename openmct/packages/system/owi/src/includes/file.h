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

#define  FILE_DATA_FLAG_ADD         1
#define  FILE_DATA_FLAG_UPDATE      2
#define  FILE_DATA_FLAG_LIST        4
#define  FILE_DATA_FLAG_DONTFILL    8
#define  FILE_DATA_FLAG_ID         16
#define  FILE_DATA_FLAG_CRYPT      32
#define  FILE_DATA_FLAG_SKIP_EMPTY 64

int file_open(char *filename);
int proc_open(char *filename);
int file_read(FILE *fp);
char *file_line_get(int line_number);
void file_print(FILE *fp);
void file_save(char *filename);
void file_free();
void file_line_action(int mode, int line_index, char *format, ...);
int file_get_pad(char *string, char *match);
int proc_open(char *command);
char *proc_read_line(char *command, int index);
char *file_read_line(char *file, int index);

extern int file_line_counter;

struct file_data_t {
   int  type;
   int  position;
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

void file_data_read(struct file_data_t *ini, char *separator);
void file_data_update(struct file_data_t *ini, char *separator);
void file_data_update_detail(struct file_data_t *ini, int line_index, char *separator);
void file_data_detail(struct file_data_t *ini, char **entry);
void file_data_update_column(struct file_data_t *column, int line_index, char **entry);
