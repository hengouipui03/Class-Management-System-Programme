#include "operation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// We input our chosen ID
int query(char *input) {
    str_clean(input);

    if (count() == 0) {     // check hashtable for active student records stored
        // validation: no student records in database
        printf("CMS: There are no student records to query.\n");
        return 1;
    }

    // finds "ID=" from admin input via parsing using strstr
    // strstr: finds first occurence of substring in string & returns a pointer - <string.h>
    char *ptrid = strstr(input, "ID=");
    
    // Check if "ID=" is missing
    if (ptrid == NULL) {
        printf("CMS: Missing \"ID=\". Please type: \"QUERY ID=\"\n");
        return 1;
    }

    char idstr[8];
    int idLen = strlen(ptrid + 3);
    
    // Check if no ID was provided after "ID="
    if (idLen == 0) {
        printf("CMS: No student ID given. Please enter a student ID.\n"); // validation: no student ID given by admin
        return 1;
    }

    strncpy(idstr, ptrid + 3, idLen);
    idstr[idLen] = '\0';  // Add null terminator for safety
    
    for (int i = 0; idstr[i]; i++){
        if(!isdigit(idstr[i])){
            printf("CMS: Invalid student ID format. Please enter a numeric ID.\n");
            return 1;   
        }
    }

    // atoi: ASCII to integer
    int id = atoi(idstr); //format ID first
    Student *result = hashLookup(id);   // lookup in the hashtable
 
    // If none is found we print that nothing is found
    if (result == NULL) {
        printf("CMS: The record with ID=%d does not exist.\n", id);     // validation: student record does not exist
    } else {
        // else, print it out and show what we found.
        printf("CMS: The record with ID=%d is found in the table.\n", id);
        printf("%-*s %-*s %-*s %-*s\n", 
            maxID,"ID", maxName+1, "Name", 
            maxProgramme+1, "Programme", maxMark, "Mark");
 
        printf("%-*d %-*s %-*s %-*.1f\n", 
            maxID, result->keyID, 
            maxName+1, result->name, 
            maxProgramme+1, result->programme, 
            maxMark, result->mark,
            result->grade);
    }
    return 0;   // found, return True
}