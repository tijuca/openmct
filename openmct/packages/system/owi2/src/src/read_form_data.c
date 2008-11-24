/* read_form_data.c
 * Copyright (c) 2008 Carsten Schoenert (claxan-com@online.de)
 * 
 *this file is part of the OWI2 for OpenMCT (http://www.openmct.org)
 * 
 * all the stuff to read and temporaly store POST or GET data from some html <form> ... </form>
 */

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "owi2.h"

struct CGI_DATA *end = NULL;

/*  function: reading data from the POST or GET methode in
 *  return: string puffer with the data
 *  return on error : NULL
 */
char *getdata(void) {
    unsigned long size;
    char *puffer = NULL;
    char *request = getenv("REQUEST_METHOD");
    char *cont_len;
    char *cgi_string;

    // first check the request methode
    if( NULL == request )
        return NULL;
    else if( strcmp(request, "GET") == 0 ) { // we have data via GET
#ifdef DEBUG
        extern char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] getting CGI-DATA via GET\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
        cgi_string = getenv("QUERY_STRING");
        if( NULL == cgi_string )
            return NULL;
        else {
            puffer = (char *) Strdup(cgi_string);
#ifdef DEBUG
        //char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] got GET CGI-DATA %s\n",__FUNCTION__,__LINE__,puffer);
        debug_info(debug_buf);
#endif
            return puffer;
        }
    }
    else if( strcmp(request, "POST") == 0 ) { // we have data via POST
#ifdef DEBUG
        extern char debug_buf[600];   // for debugging output
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] getting CGI-DATA via POST\n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
#endif
        // we have to know how long the POST string is because data comes from stdin
        // we read out the env (CONTENT_LENGTH)
        cont_len = getenv("CONTENT_LENGTH");
        if( NULL == cont_len)
            return NULL;
        else {
            size = (unsigned long) atoi(cont_len); //* casting CONTENT_LENGTH to unsigned long
            if(size <= 0)
                return NULL; // there was no data! :-(
        }
        // o.k. we have data
        puffer =(char *) malloc(size+1);
        if( NULL == puffer )
            return NULL;
        else {
            if( NULL == fgets(puffer, size+1, stdin) ) {
                free(puffer);
                return NULL;
            } else 
            {
/*#ifdef DEBUG
                //char debug_buf[600];   // for debugging output
                snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] got POST CGI-DATA %s\n",__FUNCTION__,__LINE__,puffer);
                debug_info(debug_buf);
#endif*/
                return puffer;
            }
        }
    }
    // we can't get any data
    else
        return NULL;
}

/* function: our own string duplicate function because 'strdup()' in '<string.h>'
 * is not a ANSI C funktion!
 * return: char string
 */
char *Strdup(const char *str) {
    char *p;
    if(NULL == str)
       return NULL;
    else {
        p = (char *)malloc(strlen(str)+1);
        if(NULL == p)
            return NULL;
        else
            strcpy(p, str);
    }
    return p;
}

/* function: change single hex strings (%xx) to ASCII-chars and change coded spaces (+) to real spaces
 * return: nothing 
 */
void hex2ascii(char *str) {
    int x,y;
    for(x=0,y=0; str[y] != '\0'; ++x,++y) {
        str[x] = str[y];
        // one hexadezimale char?
        if(str[x] == '%')  {
            str[x] = convert(&str[y+1]);
            y += 2;
        }
        // is it a coded space?
        else if( str[x] == '+')
            str[x]=' ';
    }
    str[x] = '\0'; // clearly terminating the string
}

/* function: converting a string of two hex chars to ascii
 * return: the converted ascii string
 */
char convert(char *hex) {
    char ascii;
    // first hex value
    ascii = (hex[0] >= 'A' ? ((hex[0] & 0xdf) - 'A')+10 : (hex[0] - '0'));
    ascii <<= 4; // bitshifting is faster then ascii*=16
    // second hex value
    ascii += (hex[1] >= 'A' ? ((hex[1] & 0xdf) - 'A')+10 : (hex[1] - '0'));
    return ascii;
}

/* function: create a list of variable/value
 * return: starting adress of the list
 * return on error: NULL
 */
struct CGI_DATA *create_cgi_list(char *str) {
    char* s;
    char* res;
    // there is of course a maximum! we allow MAX_PAIRS
    char *pairs[MAX_PAIRS];
    struct CGI_DATA *head = NULL;
    struct CGI_DATA *ptr_start = NULL;
    
    int i=0, j=0;

    // first we split the variables/values on the char '&' if there are more then one value
    s=str;
    res=strtok(s,"&");
    while( res != NULL && i < MAX_PAIRS) {
        // the value of 'res' dynamicly store in char **pairs
        pairs[i] = (char *)malloc(strlen(res)+1);
        if(pairs[i] == NULL)
            return NULL;
        pairs[i] = res;
        res=strtok(NULL,"&");
        i++;
    }
    // storing the splitted data in the struct CGI_DATA 
    while ( i > j )  { // the first variable/value
        if(ptr_start == NULL) {
            ptr_start =(struct CGI_DATA *)
            malloc(sizeof (struct CGI_DATA *));
            if( ptr_start == NULL )
                return NULL;
            res = strtok( pairs[j], "=");
            if(res == NULL)
                return NULL;
            ptr_start->variable = (char *) malloc(strlen(res)+1);
            if( ptr_start->variable == NULL )
                return NULL;
            ptr_start->variable = res;
            res = strtok(NULL, "\0");
            if(res == NULL)
                return NULL;
            ptr_start->value = (char *) malloc(strlen(res)+1);
            if( ptr_start->value == NULL )
                return NULL;
            ptr_start->value = res;
#ifdef DEBUG
            char debug_buf[600];   // for debugging output
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] variable: \"%s\" value: \"%s\"\n",
                                                        __FUNCTION__,__LINE__,ptr_start->variable,  ptr_start->value);
            debug_info(debug_buf);
#endif
            /* printf("%s %s<br>",
            * ptr_start->variable, ptr_start->value); */
            ptr_start->next =(struct CGI_DATA *) malloc(sizeof (struct CGI_DATA *));
            if(ptr_start->next == NULL)
                return NULL;
            head = ptr_start->next;
            j++;
        } else 
        { // and now the rest of the elements
            res = strtok( pairs[j], "=");
            if(res == NULL)
                return NULL;
            head->variable =(char *)
            malloc(strlen(res)+1);
            if(head->variable == NULL)
                return NULL;
            head->variable = res;
            res = strtok(NULL, "\0");
            if(res == NULL)
                return NULL;
            head->value =(char *) malloc(strlen(res)+1);
            if(head->value == NULL)
                return NULL;
            head->value = res;
#ifdef DEBUG
            char debug_buf[600];   // for debugging output
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] variable: \"%s\" value: \"%s\"\n",__FUNCTION__,__LINE__,head->variable,  head->value);
            debug_info(debug_buf);
#endif
            /* printf("%s %s<br>",
            * ptr_data->variable,  ptr_data->value); */
            head->next = (struct CGI_DATA *) malloc(sizeof (struct CGI_DATA *));
            if( head->next == NULL ) {
                snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] NULL Pointer\n",__FUNCTION__,__LINE__);
                debug_info(debug_buf);
                return NULL;
            }
            head = head->next;
            j++;
            //snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] variable j: %d head= %s\n",__FUNCTION__,__LINE__,j,head->next);
            //debug_info(debug_buf);
        }
    }
    head->next=NULL;
    end = head;
        
/*        if(ptr_data->value != NULL) {
        snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d] Listeneintraege : \n",__FUNCTION__,__LINE__);
        debug_info(debug_buf);
        while(ptr_data->value != NULL ){
            snprintf(debug_buf, sizeof(debug_buf)-1, "[%s/%d]  %s \n",__FUNCTION__,__LINE__,ptr_data->value);
            debug_info(debug_buf);
            ptr_data = ptr_data->next;
            }
        }*/
        
        // returning the start adress of the struct CGI_DATA
        return ptr_start;
}


void delete(struct CGI_DATA *data) {
    struct CGI_DATA *next = NULL;
    
    while(data != end) {
        next = data->next;
        if(data->variable != NULL)
            free(data);
        data=next;
    }
}
