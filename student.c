/* student.c: routines for loading student information */
#include "student.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "eprintf.h"
#include "templates.h"

/* FIELDS: ordering of student fields in CSV file: */
enum FIELDS {
    NAME = 0,               /* BLOGGS Fred */
    SEX = 1,                /* M */
    ATTITUDEGRADE = 2,      /* R */
    FIRSTSENTENCE = 3,      /* following fields are template keys */
    SUCCESS1 = 4,
    SUCCESS2 = 5,
    IMPROVE = 6,
    ATTITUDE = 7,
    LASTSENTENCE = 8
};

/* loadname: load name from csv line and store in student.
 Name will be in the form LAST (LAST) First (First)
 We take the first position where there is a word with lower case char in it. */
static void loadname(Student *s, CSV *csv)
{
    char *p = csv_field(csv, NAME);
    
    if (NULL == p || *p == '\0')
        eprintf("student does not have a name in line: %s", csv_line(csv));
    
    while (!islower(*p) && *p != '\0') {
        p++;
    }
    
    s->name = p-1;
}

static void loadsex(Student *s, CSV *csv)
{
    char *sex;
    
    sex = csv_field(csv, SEX);
    if (NULL == sex || sex[0] == '\0')
        eprintf("cannot load sex from line: %s", csv_line(csv));
    if ('M' == toupper(sex[0]))
        s->ismale = 1;
    else
        s->ismale = 0;
}

static void loadattitude(Student *s, CSV *csv)
{
    char *a;
    
    a = csv_field(csv, ATTITUDEGRADE);
    if (NULL == a || a[0] == '\0')
        eprintf("cannot load attitude grade from line: %s", csv_line(csv));
    if (strlen(a) > 1) /* if it's RI we only want the R */
        *(a+1) = '\0';
    if (NULL == (s->attitudeprefix = templatesget(a)))
        eprintf("cannot find attitude prefix for key: %s", a);
    
    a = csv_field(csv, ATTITUDE);
    if (NULL == a || a[0] == '\0')
        eprintf("cannot load attitude key from line: %s", csv_line(csv));
    if (NULL == (s->attitude = templatesget(a)))
        eprintf("cannot find attitude statement for key: %s", a);
}

static void loadsuccesses(Student *s, CSV *csv)
{
    char *a;
    
    a = csv_field(csv, SUCCESS1);
    if (NULL == a || a[0] == '\0')
        eprintf("cannot load success key from line: %s", csv_line(csv));
    if (NULL == (s->success1 = templatesget(a)))
        eprintf("cannot find success statement for key: %s, line: %s", a, csv_line(csv));
    
    a = csv_field(csv, SUCCESS2);
    if (NULL != a && a[0] != '\0') {
        if (NULL == (s->success2 = templatesget(a)))
            eprintf("cannot find success2 statement for key: %s, line: %s", a, csv_line(csv));
    }
    else
        s->success2 = NULL;
}

static void loadimprovements(Student *s, CSV *csv)
{
    char *a;
    
    a = csv_field(csv, IMPROVE);
    if (NULL == a || a[0] == '\0')
        eprintf("cannot load improvement key from line: %s", csv_line(csv));
    if (NULL == (s->improve = templatesget(a)))
        eprintf("cannot find improvement for key %s, line %s", a, csv_line(csv));
}

static void loadfirstsentence(Student *s, CSV *csv)
{
    char *a = csv_field(csv, FIRSTSENTENCE);
    if (NULL == a || *a == '\0')
        s->firstsentence = NULL;
    else {
        if (NULL == (s->firstsentence = templatesget(a)))
            eprintf("cannot find first sentence for key %s, line %s", a, csv_line(csv));
    }
}

static void loadlastsentence(Student *s, CSV *csv)
{
    char *a = csv_field(csv, LASTSENTENCE);
    if (NULL == a || *a == '\0')
        s->lastsentence = NULL;
    else {
        if (NULL == (s->lastsentence = templatesget(a)))
            eprintf("cannot find last sentence for key %s, line %s", a, csv_line(csv));
    }
}

Student *studentload(Student *s, CSV *csv)
{
   char *line;
   
   if (-1 == csv_read(csv))
       return NULL;
   
    loadname(s, csv);
    loadsex(s, csv);
    loadattitude(s, csv);
    loadfirstsentence(s, csv);
    loadsuccesses(s, csv);
    loadimprovements(s, csv);
    loadlastsentence(s, csv);
    return s;
}
