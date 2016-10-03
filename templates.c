/* templates.c: statement templates handling */
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "eprintf.h"

static char **templatekeys = NULL;  /* template key pointers */
static char **templates = NULL;     /* template statement pointers */
static int  maxtemplates = 0;       /* size of templates arrays */
static int  ntemplates = 0;         /* number of templates */

/* reset: back to original state */
static void reset(void)
{
    int i;
    for (i=0; i<ntemplates; i++) {
        free(templatekeys[i]);
        free(templates[i]);
    }
    free(templatekeys);
    free(templates);
    templatekeys = templates = NULL;
    maxtemplates = ntemplates = 0;
}

/* grow: grow the arrays (double each time) */
static void grow(void)
{
    char **newkeys, **newtemplates;
    
    if (maxtemplates == 0)
        maxtemplates = 1;
    else
        maxtemplates *= 2;
    newkeys = (char **) realloc(templatekeys, maxtemplates * sizeof(char*));
    newtemplates = (char **) realloc(templates, maxtemplates * sizeof(char*));
    if (NULL == newkeys || NULL == newtemplates)
        eprintf("failed to allocate storage for templates:");
    templatekeys = newkeys;
    templates = newtemplates;
}

/* templatesload: load template information from file */
void templatesload(FILE *fin)
{
    char *line, *key, *template, *tmpkey, *tmptemplate;
    CSV csv;
    
    csv_init(&csv, fin);
    
    while (-1 != csv_read(&csv)) {
        if (NULL == (tmpkey = csv_field(&csv, 0)))
            eprintf("cannot read key from line %s", line);
        if (NULL == (tmptemplate = csv_field(&csv, 1)))
            eprintf("cannot read value from line %s", line);
        key = estrdup(tmpkey);
        template = estrdup(tmptemplate);
        
        if (ntemplates >= maxtemplates)
            grow();
        templatekeys[ntemplates] = key;
        templates[ntemplates++] = template;
    }  
}

/* templatesget: find template with key */
char *templatesget(char *key)
{
    int i;
    for (i=0; i<ntemplates; i++) {
        if (0 == strcmp(key, templatekeys[i]))
            return templates[i];
    }
    return NULL;
}
