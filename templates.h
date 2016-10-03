/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT license.
 *
 * templates.h: statement template handling. 
 */

#ifndef TEMPLATES_H
#define TEMPLATES_H

#ifdef __cplusplus
extern "C" {
#endif

/* templatesload: load template data from fin */
extern void templatesload(FILE *fin);

/* templatesget: find and return the template matching key.
 returns NULL if there is no matching template.
 Caller must copy data if it is to be modified. */
extern char *templatesget(char *key);


#ifdef __cplusplus
}
#endif

#endif /* TEMPLATES_H */

