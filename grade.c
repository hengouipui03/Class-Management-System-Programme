#ifndef GRADE_C
#define GRADE_C

#include "operation.h"
#include <string.h>

void assignGrade(Student *st) {         // called by many other different operations for displaying of the grade (display operations: Michael)
    if (!st) return;
    if      (st->mark >= 85) strcpy(st->grade, "A+");
    else if (st->mark >= 80) strcpy(st->grade, "A");
    else if (st->mark >= 75) strcpy(st->grade, "A-");
    else if (st->mark >= 70) strcpy(st->grade, "B+");
    else if (st->mark >= 65) strcpy(st->grade, "B");
    else if (st->mark >= 60) strcpy(st->grade, "B-");
    else if (st->mark >= 55) strcpy(st->grade, "C+");
    else if (st->mark >= 50) strcpy(st->grade, "C");
    else if (st->mark >= 45) strcpy(st->grade, "D+");
    else if (st->mark >= 40) strcpy(st->grade, "D");
    else                     strcpy(st->grade, "F");
}

// called when a new student mark is inserted or updated

#endif