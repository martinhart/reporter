/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT license.
 * 
 * report.h: report generation routines.
 */
#ifndef REPORT_H
#define REPORT_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "student.h"

/* reportstudent: generate report for student and write to file */
extern void reportstudent(Student *, FILE *);


#ifdef __cplusplus
}
#endif

#endif /* REPORT_H */

