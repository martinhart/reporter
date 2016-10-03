/* report.c: report generation routines */

#include "student.h"
#include <stdlib.h>
#include <string.h>
#include "eprintf.h"
#include "pluralise.h"

/* sexualise: write a string to file changing HE to he|she and HIS to his|her */
static void sexualise(int ismale, FILE *f, char *template)
{
    int i, len;
    
    len = strlen(template);
    for (i=0; i<len; i++) {
        if (template[i] == 'H' && i<len) {
            if (template[i+1] == 'E') { /* this is HE */
                if (ismale)
                    fprintf(f, "he");
                else
                    fprintf(f, "she");
                i += 1;
            }
            else if (template[i+1] == 'I') { /* could be HIS */
                if (i+1 < len && template[i+2] == 'S') {
                    if (ismale)
                        fprintf(f, "his");
                    else
                        fprintf(f, "her");
                    i += 2;
                }
                else /* must be equal to HI - print verbatim */
                    fputc(template[i], f);
            }
            else /* print verbatim */
                fputc(template[i], f);
        }
        else /* print verbatim */
            fputc(template[i], f);
    }
}

/* write_sentence: substitute NAME and sex in sentence */
static void write_sentence(FILE *f, Student *student, char *s)
{
    char *p, *q;
    
    if (NULL == (p = strstr(s, "NAME")))
        sexualise(student->ismale, f, s);
    else {
        *p = '\0';
        sexualise(student->ismale, f, s);
        q = p+4; /* skip NAME and trailing space */
        fprintf(f, "%s", student->name);
        sexualise(student->ismale, f, q);
        *p = 'N'; /* put back N of name */
    }
}

/* write_first_sentence_if_needed: if there is one, write it! */
static void write_first_sentence_if_needed(Student *s, FILE *f)
{
    if (s->firstsentence != NULL && *s->firstsentence != '\0') {
        write_sentence(f, s, s->firstsentence);
        fprintf(f, "  ");
    }
}

/* write_last_sentence_if_needed: if there is one, write it! */
static void write_last_sentence_if_needed(Student *s, FILE *f)
{
    if (s->lastsentence != NULL && *s->lastsentence != '\0') {
        fprintf(f, "  ");
        write_sentence(f, s, s->lastsentence);
    }
}


/* write_success: write a statement about success 
 * f: file 
 * prefix: NAME | He | She
 * ismale: used to sexualise the template
 * template: gumpf to write
 */
static void write_success(FILE *f, char *prefix, int ismale, char *template)
{
    int n;
    char *p, *q, *plural;
    
    fprintf(f, "%s ", prefix);
    n = rand() % 4;
    if (n == 0) {
        fprintf(f, "can ");
        sexualise(ismale, f, template);
    }
    else if (n == 1) {
        fprintf(f, "is able to ");
        sexualise(ismale, f, template);
    }
    else if (n == 2) {
        sexualise(ismale, f, "has shown that HE can ");
        sexualise(ismale, f, template);
    }
    else {
        p = estrdup(template);
        q = p;
        while (*q != ' ')
            q++;
        *q = '\0';
        fprintf(f, "%s ", pluralise(p));
        sexualise(ismale, f, ++q);
        free(p);
    }
}

/* write success1: write statements about how successful s is 
 * %s %s %s %s
 * 1: NAME
 * 2: can | is able to | has shown that HE can | <pluralize>
 * 3: success1
 */
static void write_success1(Student *s, FILE *f)
{
    write_success(f, s->name, s->ismale, s->success1);
}

/* write_success2: write statements about how successful s is */
static void write_success2(Student *s, FILE *f)
{
    if (s->ismale)
        write_success(f, "He", s->ismale, s->success2);
    else
        write_success(f, "She", s->ismale, s->success2);
}

/* write_improvements: write a sentence about how s can improve
    %s %s %s %s
    1: To improve | To make further progress | In order to progress further
    2: NAME
    3: should | must | needs to 
    4: statement template
 */
static void write_improvements(Student *s, FILE *f)
{
    int n;
    
    /* first write the prefix */
    n = rand() % 3;
    if (n == 0)
        fprintf(f, "To improve, %s ", s->name);
    else if (n == 1)
        fprintf(f, "To make further progress, %s ", s->name);
    else
        fprintf(f, "In order to progress further, %s ", s->name);
    
    /* now write the next bit */
    n = rand() %3;
    if (n == 0)
        fprintf(f, "should ");
    else if (n == 1)
        fprintf(f, "must ");
    else
        fprintf(f, "needs to ");

    sexualise(s->ismale, f, s->improve);
}

/* write_attitude_prefix: write the template line, substituting name of student */
static void write_attitude_prefix(Student *s, FILE *f)
{
    char *p, *q;
    
    if (NULL == (p = strstr(s->attitudeprefix, "NAME")))
        fprintf(f, "%s", s->attitudeprefix);
    else {
        *p = '\0';
        q = p + 4; /* skip past name and trailing space */
        fprintf(f, "%s%s%s ", s->attitudeprefix, s->name, q);
        *p = 'N'; /* put back the N for next student */
    }
    
}

/* write_attitude: write the template line, substituting HIS for his|her */
static void write_attitude(Student *s, FILE *f)
{
    char *p, *q, *r, *attitude;
    
    attitude = estrdup(s->attitude);
    p = attitude;
    while (NULL != (q = strstr(p, "HIS"))) {
        *q = '\0';
        r = q + 3; /* skip past HIS and space */
        fprintf(f, "%s%s", p, s->ismale ? "his" : "her");
        p = r;
    }
    fprintf(f, "%s", p);
    
    free(attitude);
}


void reportstudent(Student *s, FILE *f)
{
    write_first_sentence_if_needed(s, f);
    write_success1(s, f);
    fprintf(f, "  ");
    if (NULL != s->success2) {
        write_success2(s, f);
        fprintf(f, "  ");
    }
    write_improvements(s, f);
    fprintf(f, "  ");
    write_attitude_prefix(s, f);
    write_attitude(s, f);
    write_last_sentence_if_needed(s, f);
}

