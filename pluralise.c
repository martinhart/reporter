/* pluralise.c: routines to pluralise a word */
#include <stdlib.h>
#include <string.h>
#include "eprintf.h"

static char *plural = NULL;     /* local copy of result */

/* TODO: this library should hold a set of exceptions (could load from file) */

char *pluralise(char *word)
{
    int length;
    
    length = strlen(word);
    free(plural);
    plural = (char*) emalloc((length + 2) * sizeof(char));
    strcpy(plural, word);
    plural[length] = 's';
    plural[length + 1] = '\0';
    return plural;
}
