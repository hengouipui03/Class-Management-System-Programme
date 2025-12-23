//save.c - SAVE function
// Saves all records to P7_1-CMS.txt file

#include "operation.h"
#include <stdio.h>
#include <stdlib.h>

void save(void) {
    char check[10];

    if (hash_table == NULL) {
        printf("CMS: Error - Student database is not initialized.\n");
        return;
    }

    // Check if hash table is empty
    int empty = 1;
    for (int i = 0; i < STUDENT_SIZE; i++) {
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE) {
            empty = 0;
            break;
        }
    }

    // If empty → ask for confirmation
    while (empty) {
    // ask confirmation from the admin
    printf("CMS: The student table is empty. Do you still want to save it? (Y/N): ");
    printf("Type \"Y\" to Confirm or type \"N\" to cancel.\n"
            "P7_1: ");

    fgets(check, sizeof(check), stdin);
    check[strcspn(check, "\n")] = 0;  // remove new line at the end when using fgets()

        // admin's input must be case-sensitive: Y or N only
        if (strcmp(check, "N") == 0) {
            printf("CMS: The save is cancelled.\n");    
            return;

        // N detected: cancel delete action
        } else if (strcmp(check, "Y") == 0) {
            printf("CMS: Proceeding to save empty student table.\n");
            break;
            
        } else {
            // validation: invalid input from admin - prompt admin until input follows Appendix A
            printf("CMS: Invalid input. Please type 'Y' or 'N'.\n");
        }
    }

    FILE *fp = fopen("P7_1-CMS.txt", "w");
    if (fp == NULL) {
        printf("Unable to save as P7_1-CMS.txt.\n");
        return;
    }

    fprintf(fp,
        "Database Name: P7_1-CMS\n"
        "Authors: Lab P7 Grp 1\n"
        "Table Name: StudentRecords\n"
        "ID,Name,Programme,Mark\n");


    // Loop through the hash table
    for (int i = 0; i < STUDENT_SIZE; i++) {
        Student *st = hash_table[i];

        // Skip empty nodes
        if (st == NULL || st == DELETED_NODE) {
            continue;
        }

        // Write the student data to file
        fprintf(fp, "%d,%s,%s,%.1f\n",
                st->keyID,
                st->name,
                st->programme,
                st->mark);
    }

    fclose(fp);
    printf("CMS: Database saved to 'P7_1-CMS.txt'.\n");
}