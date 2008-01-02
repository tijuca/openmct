#define array_key(array, index) \
	(array)->element[index]->key

#define array_value(array, index) \
	(array)->element[index]->value

#define array_element(array, index) \
	(array)->element[index]

#define array_add(array, value) \
	array_key_add(array, NULL, value)

#define array_length(array) \
	(array)->length

#define string_split(value, separator) \
	string_split_value(value, string_value(separator))

#define string_join(array, separator) \
	string_join_value(array, string_value(separator))

#define array_key_get(array, string) \
	array_key_get_value(array, string)

enum STRING_SPLIT_STATE { STRING_SPLIT_PRE, STRING_SPLIT_ADD, STRING_SPLIT_INVALID, STRING_SPLIT_DATA, STRING_SPLIT_END };


struct array_element_t {
   struct string_t *key;
   struct string_t *value;
};

struct array_t {
   unsigned int length;
   struct array_element_t **element;
};

struct array_t *string_split_value(struct string_t *value, char *separator);
struct string_t *string_join_value(struct array_t *array, char *value);
struct string_t *array_key_get_value(struct array_t *array, char *key);
void array_index_set(struct array_t *array, unsigned int index, struct string_t *value);
void array_index_del(struct array_t **array, unsigned int index);
void array_free(struct array_t **array);
void array_key_add(struct array_t **array, struct string_t *key, struct string_t *value);
