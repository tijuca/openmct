/*
 ******************************************************************************
 *
 *      Filename        : variable.h
 *
 *      Author          : andi
 *
 *      Date            : 06/08/08
 *
 *      Description     : variable handling for template engine / header
 *
 *      Revision history
 */

struct variable_t {
        char *name;
        char *value;
        int  level;
};

char *variable_get(char *name);
void variable_set(char *name, char *value);
void variable_free();
char *variable_ltrim(char *variable);
char *variable_filter(char *variable, char *filter);
