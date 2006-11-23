#define  FILE_MAXLINE      1024
#define  FILE_LINE_SET     1
#define  FILE_LINE_DEL     2
#define  FILE_LINE_ADD     3

#define  FILE_BASE         "../examples"

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

extern int file_line_counter;

struct file_ini_t {
   char *description;
   char *variable;
   char *valid;
   char *standard;
   char **current;
   int  found;
};
