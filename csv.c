/* csv.c: csv library */
#include "csv.h"
#include <stdlib.h>
#include <string.h>

static char fieldsep[] = ",";   /* field separator chars */
static int alloc(CSV*);		// allocate memory for structures
static int grow(CSV*);		// grow memory for structures
static int endofline(FILE*, int c); // return 1 if c is end of line separator
static char *advquoted(char *p); // quoted field, return pointer to next separator
static int split(CSV*);		// split line into fields

int csv_init(CSV *c, FILE *fin)
{
  memset(c, 0, sizeof(CSV));
  c->fin = fin;
  return 0;
}

void csv_destroy(CSV *c)
{
  if (c->fin)
    fclose(c->fin);
  free(c->line);
  free(c->sline);
  free(c->fields);
  memset(c, 0, sizeof(CSV));
}

int csv_read(CSV *csv)
{
  int i, c;

  // allocate on first call
  if ((NULL == csv->line) && (0 != alloc(csv)))
    return -1; // out of memory

  for (i=0; (c=getc(csv->fin)) != EOF && !endofline(csv->fin, c); i++) {
    if (i >= csv->maxline -1) { /* grow line */
      if (0 != grow(csv))
	return -1; // out of memory
    }
    csv->line[i] = c;
  }
  csv->line[i] = '\0';
  if (!split(csv))
    return -1;

  return (c==EOF && i==0) ? -1 : 0;
}

char *csv_line(const CSV *csv) 
{
    return csv->line;
}

char *csv_field(const CSV *csv, int n)
{
  if (n < 0 || n >= csv->nfields)
    return NULL;
  return csv->fields[n];
}

int csv_nfields(const CSV *csv)
{
  return csv->nfields;
}


static int alloc(CSV *c)
{
  c->maxline = c->maxfields = 1;
  c->line = malloc(c->maxline * sizeof(char));
  c->sline = malloc(c->maxline * sizeof(char));
  c->fields = malloc(c->maxfields * sizeof(c->fields[0]));
  if (NULL == c->line || NULL == c->sline || NULL == c->fields)
    return -1;
  return 0;
}

static int grow(CSV *c)
{
  char *newl, *news;

  c->maxline *= 2;
  newl = (char *) realloc(c->line, c->maxline);
  news = (char *) realloc(c->sline, c->maxline);
  if (NULL == newl || NULL == news)
    return -1; // out of memory

  c->line = newl;
  c->sline = news;
  return 0;
}

static int endofline(FILE *fin, int c)
{
  int eol;

  eol = (c == '\r' || c == '\n');
  if (c == '\r') {
    c = getc(fin);
    if (c != '\n' && c != EOF)
      ungetc(c, fin);
  }
  return eol;
}

static char *advquoted(char *p)
{
  int i, j;
  for (i=j=0; p[j] != '\0'; i++, j++) {
    if (p[j] == '"' && p[++j] != '"') { /* copy up to next separator or end */
      int k = strcspn(p+j, fieldsep);
      memmove(p+i, p+j, k);
      i += k;
      j += k;
      break;
    }
    p[i] = p[j];
  }
  p[i] = '\0';
  return p + j;
}

static int split(CSV *c)
{
  char *p, **newf;
  char *sepp; /* temporary separator char */
  int sepc;   /* temporary separator char */

  c->nfields = 0;
  if (c->line[0] == '\0')
    return 0;
  strcpy(c->sline, c->line);
  p = c->sline;

  do {
    if (c->nfields >= c->maxfields) {
      c->maxfields *= 2;
      newf = (char **) realloc(c->fields, c->maxfields * sizeof(c->fields[0]));
      if (NULL == newf)
	return 0; /* out of memory */
      c->fields = newf;
    }

    if (*p == '"')
      sepp = advquoted(++p); /* skip initial quote */
    else
      sepp = p + strcspn(p, fieldsep);
    sepc = sepp[0];
    sepp[0] = '\0'; /* terminate field */
    c->fields[c->nfields++] = p;
    p = sepp + 1;
  } while(sepc == ',');

  return c->nfields;
}
