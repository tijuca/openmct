enum ARGUMENT_STATE { ARGUMENT_PRE, ARGUMENT_DATA, ARGUMENT_ADD, ARGUMENT_END, ARGUMENT_INVALID, ARGUMENT_MEMORY };

#define ARGUMENT_SEPERATOR_STANDARD " \t"
#define ARGUMENT_SEPERATOR_CGI      "&;"
#define ARGUMENT_SEPERATOR_CSV      ";"

char **argument_parse(char *value, char *seperator);
void argument_free(char **argument);
char *argument_get(char **argument, int index, char *seperator);

extern char **argument;
