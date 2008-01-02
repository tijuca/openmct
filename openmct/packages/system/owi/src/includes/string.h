#define STRING_DEFAULT_LENGTH		1024

#define string_init(size) \
	string_move(NULL, 0, 0, size, NULL)

#define string_copy(source) \
	string_move(NULL, 0, 0, source ? strlen(string_value(source)) : 0, source ? string_value(source) : "")

#define string_copy_value(value) \
	string_move(NULL, 0, 0, value ? strlen(value) : 0, value ? value : "")

#define string_ncopy(source, count) \
	string_move(NULL, 0, 0, count, string_value(source))

#define string_ncopy_value(value, count) \
	string_move(NULL, 0, 0, count, value)

#define string_append(destination, source) \
	string_move(destination, destination ? string_length(destination) : 0, 0, string_length(source), string_value(source))

#define string_append_value(destination, value) \
	string_move(destination, string_length(destination), 0, strlen(value), value)

#define string_insert(destination, destination_index, source) \
	string_move(destination, destination_index, 0, string_length(source), string_value(source))

#define string_insert_value(destination, destination_index, value) \
	string_move(destination, destination_index, 0, strlen(value), value)

#define string_extract(source, start, length) \
	string_move(NULL, 0, start, length, string_value(source))

#define string_extract_value(value, start, length) \
	string_move(NULL, 0, start, length, value)

#define string_compare(destination, source) \
	strcmp(string_value(destination), string_value(source))

#define string_compare_value(destination, source) \
	strcmp(string_value(destination), source)

#define string_value(string) \
	(string)->value

#define string_char_at(string, index) \
	(string)->value[index]

#define string_value_at(string, index) \
	((string)->value + index)

#define string_length(string) \
	(string)->length

#define string_match(string, pattern) \
	string_match_value(string, string_value(pattern))

struct string_t {
   unsigned int  length;
   char *value;
};

struct string_t *string_move(
   struct string_t *old,
   unsigned int destination_offset,
   unsigned int source_offset,
   unsigned int length,
   char *value);

struct string_t *string_format(char *format, ...);
void string_free(struct string_t **string);
struct string_t *string_decode(struct string_t *string);
unsigned int string_match_value(struct string_t *string, char *pattern);
