#define OWI_FLAG_CONFIG		1
#define OWI_FLAG_ACTION         2
#define OWI_FLAG_ROW            4
#define OWI_FLAG_HIDE_NEW	8
#define OWI_FLAG_ACTION_DELETE  16
#define OWI_FLAG_ACTION_DETAIL  32
#define OWI_FLAG_ACTION_KILL    64
#define OWI_FLAG_ACTION_UPDATE  128
#define OWI_FLAG_ACTION_ADD	256

#define OWI_SEPARATOR_PARAMETER ";&"
#define OWI_SEPARATOR_KEYVALUE  "="

#define OWI_ERROR_PREFIX        "error_"

struct owi_t {
   struct data_t   *data;
   struct file_t   *file;
   struct data_t   *data_init;
   struct file_t   *file_init;
   struct array_t  *request;
   struct string_t *headline;
   unsigned int    flags;
   unsigned int    index_id;
   struct string_t *(*callback)(struct owi_t *owi, struct string_t *command);
};

void owi_main(struct owi_t *owi);
void owi_header(struct owi_t *owi);
void owi_footer(struct owi_t *owi, char *button, ...);
void owi_request(struct owi_t *owi);
void owi_error_set(struct owi_t *owi, struct string_t *key, struct string_t *value);

#define owi_get(key) \
        ((owi->request && array_key_get(owi->request, key)) ? \
	array_key_get(owi->request, key) : \
	string_copy_value(""))

#define owi_get_value(key) \
        ((owi->request && array_key_get_value(owi->request, key)) ? \
	string_value(array_key_get_value(owi->request, key)) \
	: "")

#define owi_get_error(key) \
        array_key_get(owi->request, key)

#define owi_get_error_value(key) \
        string_value(array_key_get_value(owi->request, "error"))
