#include "operation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//comparing by id
int compareID(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;

    //s1 is bigger so s2 comes first
    if (s1->keyID > s2->keyID) return 1;

    //s1 is smaller so s1 comes first
    if (s1->keyID < s2->keyID) return -1; 
    return 0;
}

//comparing by mark
//to Take Note: your void * is a generic pointer, 
//at the same time qsort requires you to use "const void* "

int compareMark(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;
    
    //s1 is bigger so s2 comes first
    if (s1->mark > s2->mark) return 1;

    //s1 is smaller so s1 comes first
    if (s1->mark < s2->mark) return -1; 
    return 0;
}

int sort(char *cmpby){
    bool descending = strstr(cmpby, "DESC") != NULL;

    ///command: SHOW ALL SORT BY ID & SHOW ALL SORT BY MARK
    //You should sort by student ID (ascending or descending)
    //or by marks (ascending or descending)
    
    //sorting by descending order
    Student arr[STUDENT_SIZE];
    int arrlen = 0;

    //we want to store an array of students so we can compare later
    //via their ID or Mark respectively
    for (int i = 0; i < STUDENT_SIZE; i++) {
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE)
            arr[arrlen++] = *hash_table[i];
    }

    if (arrlen == 0) {
        printf("No students to sort.\n");
        return 0;
    }

    // choose how you want to compare, either ID or mark
    if (strstr(cmpby, "SORT BY ID") != NULL) {
        qsort(arr, arrlen, sizeof(Student), compareID);
    } else if (strstr(cmpby, "SORT BY MARK") != NULL) {
        qsort(arr, arrlen, sizeof(Student), compareMark);
    } else {
        printf("CMS: Invalid sort criteria.\n");
        return 0;
    }
    
    //once all is done we simply print out
    printf("%-*s %-*s %-*s %-*s %-s\n", 
        maxID,"ID", maxName+1, "Name", 
        maxProgramme+1, "Programme", maxMark, "Mark", "Grade");

    //this time we print out what is in our sorted array
    if (descending) {
        for (int i = arrlen - 1; i >= 0; i--) {
            printf("%-*d %-*s %-*s %-*.1f %s\n",
                maxID, arr[i].keyID,
                maxName+1, arr[i].name,
                maxProgramme+1, arr[i].programme,
                maxMark, arr[i].mark,
                arr[i].grade);
        }
    } else {
        for (int i = 0; i < arrlen; i++) {
            printf("%-*d %-*s %-*s %-*.1f %s\n",
                maxID, arr[i].keyID,
                maxName+1, arr[i].name,
                maxProgramme+1, arr[i].programme,
                maxMark, arr[i].mark,
                arr[i].grade);
        }
    }
}

void summary() {
    //command: SHOW SUMMARY
    int total = 0;
    float avgMark = 0, highestMark = -1.0, lowestMark = 101.0; //initialize highest and lowest beyond possible mark range so does not create a inf loop
    Student *highestStudent = NULL;
    Student *lowestStudent = NULL;
    
    //this will give us the total number of students
    for (int i = 0; i < STUDENT_SIZE; i++) {
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE){
            Student *st = hash_table[i];
            total++;
            avgMark += hash_table[i]->mark;

            //here we find our highest and lowest mark
            if(highestMark == -1.0 && lowestMark == 101.0){ 
                //this is to initilize our highest and lowest mark
                highestMark = hash_table[i]->mark;
                lowestMark = hash_table[i]->mark;
                highestStudent = lowestStudent = hash_table[i];
            }
            else{
                //if the mark is higher than the highest mark then its the next highest mark
                if (hash_table[i]->mark > highestMark) {
                    highestMark = hash_table[i]->mark;
                    highestStudent = hash_table[i];
                }
                //if it is lower than the lowest, its the next lowest mark
                if (hash_table[i]->mark < lowestMark) {
                    lowestMark  = hash_table[i]->mark;
                    lowestStudent = hash_table[i];
                }
            }
        }
    }

    //Just a check to ensure we actually have students to calculate with
    if (total > 0){
        //calculating average
        avgMark = avgMark / total;
    }
    Student avgTemp;
    avgTemp.mark = avgMark;
    assignGrade(&avgTemp);

    //print everything
    printf("Summary Report\n");

    printf("%-22s : %d\n", "Total students", total);
    printf("%-22s : %-6.1f %-3s\n", "Average mark", avgMark, avgTemp.grade);
    if (highestStudent != NULL)
        printf("%-22s : %-6.1f %-6s %s\n", "Highest mark", highestMark, highestStudent->grade, highestStudent->name);
    else
        printf("Highest mark: N/A\n");

    if (lowestStudent != NULL)
        printf("%-22s : %-6.1f %-6s %s\n", "Lowest mark", lowestMark, lowestStudent->grade, lowestStudent->name);
    else
        printf("Lowest mark: N/A\n");

}