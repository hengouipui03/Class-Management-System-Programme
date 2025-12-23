#include "operation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int insert(char *input){
    str_clean(input); // clean the input string first
    
    if (count() == 0) {     // check hashtable for active student records stored
        // validation: no student records in database
        printf("CMS: There are no student records to insert.\n");
        return 1;
    }

    char *ptrid = strstr(input, "ID=");
    if (ptrid) ptrid += 3;

    char *ptrName = strstr(input, " Name=");
    char *ptrProg = strstr(input, " Programme=");
    char *ptrMark = strstr(input, " Mark=");
    char missing[200] = "";
    
    if(!ptrid || !isdigit(ptrid[0]))  // Note: now ptrid[0] since we moved pointer
        strcat(missing, " ID=[StudentID]");
    
    if(!ptrName || strncmp((ptrName + 6), "Programme=", 10) == 0 || strncmp(ptrName + 6, "Mark=", 5) == 0)
        strcat(missing, " Name=[StudentName]");
    
    if(!ptrProg || strncmp((ptrProg + 11), "Mark=", 5) == 0)
        strcat(missing, " Programme=[StudentProgramme]");
    
    if(!ptrMark || !isdigit(ptrMark[6]))
        strcat(missing, " Mark=[StudentMark]");
    
    if(strlen(missing) > 0){
        printf("CMS: Missing category to insert. Provide all of the following:%s\n", missing);
        return 1;
    }

    char idstr[7];
    int digitCount = 0;
    while(isdigit(ptrid[digitCount]))digitCount++;
 
    //check if we have a valid ID
    if(digitCount != 7){
        printf("CMS: Invalid Input, ID can only be 7 digits."); 
        return 1;
    }

    strncpy(idstr, ptrid, 7);
    idstr[8] = '\0';  // null terminator

    // Validate numeric ID
    for (int i = 0; idstr[i]; i++) {
        if (!isdigit((unsigned char)idstr[i])) {
            printf("CMS: Invalid Input, ID must be numeric.\n");
            return 1;
        }
    }
 
    int id = atoi(idstr); //format ID first
 
    if(hashLookup(id) != NULL){ // Check for duplicate ID
        printf("CMS: Invalid Input, The record with ID=%d already exists.", id);
        return 1;
    }

    for (int i = 1; i < (strlen(ptrName) - strlen(ptrProg)); i++) {
        if (isalpha(ptrName[i]) == 0 && ptrName[i] != ' ' && ptrName[i] != '=') {
            printf("CMS: Invalid Input, Name cannot have a number.\n");
            return 1;
        }
    }

    for (int i = 1; i < (strlen(ptrProg) - strlen(ptrMark)); i++) {
        if (isalpha(ptrProg[i]) == 0 && ptrProg[i] != ' ' && ptrProg[i] != '=') {
            printf("CMS: Invalid Input, Programme cannot have a number.\n");
            return 1;
        }
    }
 
    float mark = atof(ptrMark + 6);
    float min_limit = 0.0f;
        float max_limit = 100.0f;
        if (mark < min_limit || mark > max_limit) {
            printf("CMS: Invalid Input, Input has invalid mark. \n",mark);
            return 1; // Skip invalid mark
        }
 
    // Allocate a new Student on the heap
    Student *tmp = malloc(sizeof(Student));
    // Set keyID
    tmp->keyID = id;
 
    // Copy name
    int nameLen = ptrProg - (ptrName + 6);
    strncpy(tmp->name, ptrName + 6, nameLen);
    tmp->name[nameLen] = '\0';

    // Update maxName
    if (nameLen > maxName) maxName = nameLen;
 
    // Copy programme
    int progLen = ptrMark - (ptrProg + 11);
    strncpy(tmp->programme, ptrProg + 11, progLen);
    tmp->programme[progLen] = '\0';

    // Update maxProgramme
    if (progLen > maxProgramme) maxProgramme = progLen;
 
    // Copy mark
    
    tmp->mark = mark;

    assignGrade(tmp);
 
    // Insert into hash table
    hashInsert(tmp);
    printf("CMS: A new record with ID=%d successfully inserted.\n", id);//confirm insertion
}
