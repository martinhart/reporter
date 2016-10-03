#ifndef CSV_H
#define CSV_H
/* csv.h: interface for csv library */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef struct CSV_ {
  FILE		*fin;		// input file
  char		*line;		// input chars
  char		*sline;		// line copy used when splitting
  int		maxline;	// size of line[] and sline[]
  char		**fields;	// pointers to fields
  int		maxfields;	// size of field
  int		nfields;	// number of items in fields
} CSV;

/**
 * csv_init: initialize the structure to default values
 * Pass in the file, which is opened for reading.
 * Returns 0 on success, -1 on error (errno may be set).
 */
extern int csv_init(CSV*, FILE *fin);

/**
 * csv_destroy: resets the structure.
 * Closes the file (if open) and frees all internal memory
 * used.
 * Do not use the structure again until csv_init is called again
 */
extern void csv_destroy(CSV*);

/**
 * csv_read: read a line from the file
 * Returns 0 on success, -1 on error (EOF or EMEM).
 */
extern int csv_read(CSV*);

/**
 * csv_field: return the field at specified index
 * Returns the actual field, or NULL if invalid index.
 * Will return an empty string if the field exists but is empty.
 * Caller must make a copy of result to keep beyond calls to csv_read.
 */
extern char *csv_field(const CSV*, int n);

/**
 * csv_nfields: return the number of fields read from last call to csv_read
 */
extern int csv_nfields(const CSV*);

/**
 csv_line: return the current line
 */
extern char *csv_line(const CSV*);


#ifdef __cplusplus
}
#endif

#endif	// CSV_H

