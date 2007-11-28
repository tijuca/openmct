#define  DATA_TYPE_TEXT      1
#define  DATA_TYPE_PASSWORD  2
#define  DATA_TYPE_CHECKBOX  3
#define  DATA_TYPE_SELECT    4
#define  DATA_TYPE_RADIO     5
#define  DATA_TYPE_TEXTAREA  6
#define  DATA_TYPE_INTERNAL  7

#define  DATA_FLAG_ADD          1
#define  DATA_FLAG_UPDATE       2
#define  DATA_FLAG_LIST         4
#define  DATA_FLAG_DONT_FILL    8
#define  DATA_FLAG_ID          16
#define  DATA_FLAG_CRYPT       32
#define  DATA_FLAG_SKIP_EMPTY  64

struct data_t {
   int  type;
   int  flags;
   char *name;
   char *description;
   char *html;
   char *directive;
   char *valid;
   char *standard;
};

int data_valid(struct data_t *data, char *value);
