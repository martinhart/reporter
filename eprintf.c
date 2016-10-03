/* eprintf.c: error routines */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* eprintf: print error message and exit */
void eprintf(char *fmt, ...)
{
    va_list       args;
    const char    *progname;
    
    fflush(stdout);
    if (NULL != (progname = getprogname()))
        fprintf(stderr, "%s: ", progname);
    
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    
    if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
        fprintf(stderr, " %s", strerror(errno));
    fprintf(stderr, "\n");
    exit(2);
}

/* efopen: open file or exit */
FILE *efopen(char *path, char *mode)
{
    FILE *fp;
    
    if (NULL == (fp = fopen(path, mode)))
        eprintf("failed to open %s with mode %s:", path, mode);
    return fp;
}

/* estrdup: copy string or exit */
char *estrdup(char *s)
{
    char *r;
    
    if (NULL == (r = strdup(s)))
        eprintf("failed to copy string: %s:", s);
    return r;
}

/* emalloc: allocate or die */
void *emalloc(size_t size)
{
    void *p;
    
    if (NULL == (p = malloc(size)))
        eprintf("failed to allocate %d bytes:", size);
    return p;
}

