#include "operation.h"
// #include "string.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int openDatabase(){
    //clean slate
    initialise();

    char line[256];
    //it tries to open the text file, if there is no file it returns an error
    //stating no file found or opened
    FILE* databaseFile = fopen("P7_1-CMS.txt", "r");
    if (!databaseFile) { perror("Unable to open file"); return 1; }

    int databasePrint = 0;
    int authorPrint = 0;
    int tablePrint = 0; // to ensure that the database name, author and table name lines are only printed once
   
    while (fgets(line, sizeof(line), databaseFile)){
        char *coloncheck = strchr(line, ':');
        if (coloncheck != NULL){
            if(strncmp(line, "Database Name:", strlen("Database Name:")) == 0 && databasePrint == 0){ //check and print database name only once
                printf("%s", line);
                databasePrint = 1;
                continue;
            }
            else if (strncmp(line, "Authors:",strlen("Authors:") ) == 0 && authorPrint == 0){ //check and print authors name only once
                printf("%s", line);
                authorPrint = 1;
                continue;
            }
            else if (strncmp(line, "Table Name:", strlen("Table Name:")) == 0 && tablePrint == 0){ //check and print table name
                printf("Opening %s", line);
                tablePrint = 1;
                continue;
            }
            else if (strncmp(line, "Table Name:", strlen("Table Name:")) == 0 && tablePrint >= 1){ //if there is more tables found
                printf("There is a another table in the CMS file. Do you want to continue opening? (Y/N):/n");
                char response;
                scanf(" %c", &response);
                getchar(); // to remove the /n after scanf
                while(1){
                    response = toupper(response);
                    if (response == 'Y' || response == 'N'){
                        break;
                    }
                    printf("Invalid input. Please enter Y or N: ");
                    scanf(" %c", &response);
                    getchar();
                }
                if (response == 'N'){
                    fclose(databaseFile);
                    return 0; // Exit without error
                }
                else {
                    printf("Opening %s", line);
                }
                tablePrint++; 
                continue;
            }
            
        }
        if(strncmp(line, "ID,Name,Programme,Mark", strlen("ID,Name,Programme,Mark")) == 0){
            continue; //skip header line
        }
        line[strcspn(line, "\n")] = 0; //fget may give it a "newline", so we want to remove it if not removed,
        //it might store the new line in the data and mess up when we eventually print out the line
        if (strlen(line) == 0){
            continue; // Skip empty lines
        }
        //start splitting string via the comma
        char *idStr = strtok(line, ",");
        //int id = atoi(strtok(line, ","));//NULL just means to continue splitting the same string,
        char *name = strtok(NULL, ",");//at the same time it will remember where we stopped splitting the previous element and continue from there.
        char *programme = strtok(NULL, ",");
        char *markStr = strtok(NULL, ",");
        float mark = markStr ? atof(markStr) : 0.0f;
        if (!idStr || !name || !programme || !markStr) {
            printf("WARNING: Line (ID=%s, Name=%s, Programme=%s, Mark=%.1f) has missing information. Skipping.\n", idStr, name, programme, mark);
            continue; // Skip bad line
        }

        if (strlen(idStr) > (maxID-1)){
                printf("WARNING: Line (ID=%s, Name=%s, Programme=%s, Mark=%.1f) has invalid ID. Skipping.\n", idStr, name, programme, mark);
                continue;
            }

        int wrongID = 0;
        for (int i = 0; idStr[i]; i++) {
            if (!isdigit(idStr[i])) {
                printf("WARNING: Line (ID=%s, Name=%s, Programme=%s, Mark=%.1f) has invalid ID. Skipping.\n", idStr, name, programme, mark);
                wrongID = 1;
                break; // Skip invalid ID;
            }
        }
        if (wrongID){
            continue;
        }
        int id = atoi(idStr); //convert string to integer
        
        if (hashLookup(id) != NULL) {
            printf("WARNING: Duplicate ID found in line (ID=%d, Name=%s, Programme=%s, Mark=%.1f). Skipping.\n", idStr, name, programme, mark);
            continue; // Skip duplicate ID
        }

        if (strlen(name) > maxName){ 
            maxName = strlen(name); //increase maxName length if current name is longer so that table formatting works
        };

        if (strlen(programme) > maxProgramme){
            maxProgramme = strlen(programme); //increase maxProgramme length if current name is longer so that table formatting works
        };
        
        float min_limit = 0.0f;
        float max_limit = 100.0f;
        if (mark < min_limit || mark > max_limit) {
            printf("WARNING: Line (ID=%d, Name=%s, Programme=%s, Mark=%.1f) has invalid mark. Skipping.\n", idStr, name, programme, mark);
            continue; // Skip invalid mark
        }

        // Allocate memory for a new Student on the heap
        Student *tmp = malloc(sizeof(Student));

        // Copy data into the new student
        tmp->keyID = id;
        strcpy(tmp->name, name);           // copy name string
        strcpy(tmp->programme, programme); // copy programme string
        tmp->mark = mark;

        assignGrade(tmp);

        // Insert the student into the hash table
        hashInsert(tmp);
    }
    fclose(databaseFile);

    return 0;
}

void updateColumnWidths() {
    // reset to minimum (header) widths
    maxName = strlen("Name");
    maxProgramme = strlen("Programme");
    
    // scan through all records and find maximum widths for new programme
    for (int i = 0; i < STUDENT_SIZE; i++) {
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE) {
            int nameLen = strlen(hash_table[i]->name);
            int progLen = strlen(hash_table[i]->programme);
            
            if (nameLen > maxName) {
                maxName = nameLen;
            }
            if (progLen > maxProgramme) {
                maxProgramme = progLen;
            }
        }
    }
}

void showAll(){
    if (count() == 0) {
        printf("CMS: There are no student records found.");
    }
    else {
        updateColumnWidths();
        printf("%-*s %-*s %-*s %-*s %-*s\n",
           maxID, "ID",
           maxName + 1, "Name",
           maxProgramme + 1, "Programme",
           maxMark, "Mark",
           7, "Grade"); // 7 width for grade column

        for (int i = 0; i < STUDENT_SIZE; i++){
            if(hash_table[i] != NULL && hash_table[i] != DELETED_NODE){
                printf("%-*d %-*s %-*s %-*.1f %-*s\n",
                    maxID, hash_table[i]->keyID,
                    maxName + 1, hash_table[i]->name,
                    maxProgramme + 1, hash_table[i]->programme,
                    maxMark, hash_table[i]->mark,
                    7, hash_table[i]->grade); // 7 is width for "Grade" column
            }
        }
    }    
}

