/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT license.
 *
 * student.h: information about students.
 */
#ifndef STUDENT_H
#define STUDENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csv.h"

typedef struct Student {
    char *name;              /* student name */
    int  ismale;             /* male or female */
    char *attitudeprefix;    /* attitude to learning prefix */
    char *firstsentence;     /* first sentence */
    char *success1;          /* success 1 key */
    char *success2;          /* success 2 key */
    char *improve;           /* improvement key */
    char *attitude;          /* attitude improve key */
    char *lastsentence;      /* last sentence */
} Student;

/* NAME,SEX,ATTITUDEGRADE,FIRSTSENTENCE,SUCCESS1,SUCCESS2,IMPROVE,ATTITUDE,LASTSENTENCE */
extern Student *studentload(Student *, CSV *);     /* read a line from file and
                                                    * store in Student structure.
                                                    * Returns NULL at EOF */

#ifdef __cplusplus
}
#endif

#endif /* STUDENT_H */

