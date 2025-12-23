#include "operation.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void delete(char *input) {
    str_clean(input);
    int digit = 0;
    char check[10];
    if (count() == 0) {     // check hashtable for active student records stored
        // validation: no student records in database
        printf("CMS: There are no student records to delete.\n");
        return;
    }

    // parse using strstr and check if "ID=" exists in admin input 
    // validation: display missing "ID=" error statement and prompt admin to input "ID"
    char *ptrid = strstr(input, "ID=");
    if (!ptrid) {
        printf("CMS: Missing \"ID=\". Please type: \"DELETE ID=\"\n");
        return;
    }

    // validation: display missing student ID and prompt admin to input a student ID
    char idstr[20];
    if (sscanf(ptrid, "ID=%19s", idstr) != 1) {
        printf("CMS: No student ID given. Please enter a student ID.\n");
        return;
    }

    // validation: check ID is only numeric numbers using isdigit
    for (int i = 0; idstr[i] && idstr[i] != ' '; i++) {
        if (!isdigit((unsigned char)idstr[i])) {
            printf("CMS: Invalid ID. ID must contain only digits.\n");
            return;
        }
        digit++;
    }
    
    // validation: check if student ID is valid with exactly 7 digits
    if (digit != 7) {
        printf("CMS: Invalid ID. ID must be exactly 7 digits.\n");
        return;
    }

    // atoi: ASCII to integer - for lookup and print the correct student ID instead of hash value
    int id = atoi(idstr);

    // check if student ID from admin input exists in hash table
    Student *student = hashLookup(id);

    if (student == NULL) {  // if student ID record does not exist
        // sample response: Appendix A
        printf("CMS: The record with ID=%d does not exist.\n", id);  // sample response: Appendix A
        return;
    }

    // else: record is found in hashtable
    while (true) {
    // ask confirmation from the admin - sample response: Appendix A
    printf("CMS: Are you sure you want to delete record with ID=%d? ", id);
    printf("Type \"Y\" to Confirm or type \"N\" to cancel.\n"
            "P7_1: ");

    fgets(check, sizeof(check), stdin);
    check[strcspn(check, "\n")] = 0;  // remove new line at the end when using fgets()

        // admin's input must be case-sensitive: Y or N only
        if (strcmp(check, "Y") == 0) {
        // Y detected: delete student id record from database
            hashDelete(id);
            printf("CMS: The record with ID=%d is successfully deleted.\n", id);  // sample response: Appendix A
            break; 

        // N detected: cancel delete action
        } else if (strcmp(check, "N") == 0) {
            printf("CMS: The deletion is cancelled.\n");    // sample response: Appendix A
            break; 
            
        } else {
            // validation: invalid input from admin - prompt admin until input follows Appendix A
            printf("CMS: Invalid input. Please type 'Y' or 'N'.\n");
        }
    }
}

