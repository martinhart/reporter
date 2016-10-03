/*
 * Copyright (c) 2016 Martin Hart under the terms of the MIT licence.
 *
 * main.c: main reporter program 
 * 
 * Read template data from templates.csv: key,value
 * Read student data from students.csv: see student.c enum for field order.
 * Write reports to reports.txt
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csv.h"
#include "eprintf.h"
#include "report.h"
#include "templates.h"
#include "student.h"

int main(int argc, char** argv) 
{
    FILE *ftemplates, *fstudents, *fout;
    Student student;
    CSV csv;
    
    srand(time(NULL));
    ftemplates = efopen("templates.csv", "r");
    fstudents = efopen("students.csv", "r");
    fout = efopen("reports.txt", "w");
    
    templatesload(ftemplates);
    fclose(ftemplates);
    
    csv_init(&csv, fstudents);
    while (-1 != csv_read(&csv)) {
        studentload(&student, &csv);
        reportstudent(&student, fout);
        fprintf(fout, "\n\n");
    }
    
    fclose(fstudents);
    fclose(fout);
    return (EXIT_SUCCESS);
}
