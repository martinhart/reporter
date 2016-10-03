/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT license.
 *
 * pluralise.h: library for pluralising words 
 */ 
#ifndef PLURALISE_H
#define PLURALISE_H

#ifdef __cplusplus
extern "C" {
#endif

/* pluralise: pass in a word and this routine returns its plural.
 * Caller must copy the result if you intend it to persist between calls, or
 * if you want to modify it.
 */
extern char *pluralise(char *word);

#ifdef __cplusplus
}
#endif

#endif /* PLURALISE_H */

