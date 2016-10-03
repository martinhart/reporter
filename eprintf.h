/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT license.
 */
#ifndef EPRINTF_H
#define EPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>

extern void eprintf(char *fmt, ...); /* print error and exit */

extern FILE *efopen(char *path, char *mode);/* open file or exit */

extern char *estrdup(char *); /* copy string or exit */

extern void *emalloc(size_t); /* allocate or die */

#ifdef __cplusplus
}
#endif

#endif /* EPRINTF_H */

