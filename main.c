// import hash table operations
#include "operation.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// declaring global variables
int maxID = 8;
int maxName;
int maxProgramme;
int maxMark;
Student *hash_table[STUDENT_SIZE];
void menu();

int main() {
    maxName = strlen("Name");
    maxProgramme = strlen("Programme");
    maxMark = strlen("Mark");
    char input[200];
    int running = 1;

    // print declaration upon start-up of program
    printf("%*s\n", 130, "Declaration");
    printf("SIT's policy on copying does not allow the students to copy source code as well as assessment solutions from another person AI or other places. "
           "It is the students' responsibility to guarantee that their assessment solutions are their own work. "
           "Meanwhile, the students must also ensure that their work is not accessible by others. "
           "Where such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n\n");
    printf("We hereby declare that:\n"
           "- We fully understand and agree to the abovementioned plagiarism policy.\n"
           "- We did not copy any code from others or from other places\n"
           "- We did not share our codes with others or upload to any other places for public access and will not do that in the future.\n"
           "- We agree that our project will receive Zero mark if there is any plagiarism detected.\n"
           "- We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.\n"
           "- We agree that we did not copy any code from AI generated sources.\n\n");
    printf("Declared by: P7-1\n"
           "Team Members:\n"
           "1. Ong Heng Hui (2501061)\n"
           "2. Poh Yang Jie Michael (2500624)\n"
           "3. Rayner Lim Yi Ke (2500611)\n"
           "4. Soh Jia Hui (2502218)\n"
           "5. Soh Wenjie (2503504)\n");
    printf("Date: ______\n");
    menu();
    
    while (running) {
        printf("\nP7_1: ");  // print database file
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;    // remove new line at the end when using fgets()
        
        // using case-sensitive match for admin's input
        if (strcmp(input, "OPEN") == 0) {
            openDatabase();
        }
        else if (strncmp(input, "SHOW ALL SORT BY", 16) == 0) {
            sort(input);
        }
        else if (strcmp(input, "SHOW ALL") == 0) {
            showAll();
        }
        else if (strncmp(input, "INSERT", 6) == 0) {
            insert(input);
        }
        else if (strncmp(input, "QUERY", 5) == 0) {
            query(input);
        }
        else if (strncmp(input, "UPDATE", 6) == 0) {
            update(input);
        }
        else if (strncmp(input, "DELETE", 6) == 0) {
            delete(input);
        }
        else if (strcmp(input, "SAVE") == 0) {
            save();
        }
        else if (strcmp(input, "SHOW SUMMARY") == 0) {
            summary();
        }
        else if (strcmp(input, "USER MENU") == 0) {
            menu();
        }
        else if (strcmp(input, "EXIT") == 0) {
            printf("CMS: Do you want to save the database before exiting? (Y/N): ");
            char response;
            scanf(" %c", &response);
            getchar(); // to remove the /n after scanf
            while(1){
                response = toupper(response);
                if (response == 'Y'){
                    save();
                    break;
                } else if (response == 'N'){
                    break;
                }
                printf("Invalid input. Please enter Y or N: ");
                scanf(" %c", &response);
                getchar();
            }
            running = 0;
        }
        else {
            printf("CMS: Unknown command.\n");
        }
    }
    
    printf("CMS: Exiting program.\n");
    return 0;
}

void menu() {
    printf("------------------------------------ CMS Main Menu ----------------------------------\n"
                "Available Commands:\n"
                "1. OPEN - Open the database file.\n"
                "2. SHOW ALL - Display all student records.\n"
                "3. SHOW ALL SORT BY [ID/MARK] [DESC] (Defaults to ASC if not specified) - Display all student records sorted by ID or Mark in ascending or descending order.\n"
                "4. INSERT ID=[StudentID] Name=[StudentName] Programme=[ProgrammeName] Mark=[StudentMark] - Insert a new student record.\n"
                "5. QUERY ID=[StudentID] - Query a student record by ID.\n"
                "6. UPDATE ID=[StudentID] Name=[StudentName] Mark=[NewMark] Programme=[NewProgramme] - Update a student record's name, mark and/or programme.\n"
                "7. DELETE ID=[StudentID] - Delete a student record by ID.\n"
                "8. SAVE - Save the current database to file.\n"
                "9. SHOW SUMMARY - Display summary statistics of student records.\n"
                "10. EXIT - Exit the program.\n"
                "11. USER MENU - Show instructions for Class Management System.\n"
                "--------------------------------------------------------------------------------\n");
}

// import hashtable
#include "hashtable.c"

// import core operations (change accordingly later.)
#include "database.c"   //open, showall
#include "query.c" 
#include "insert.c"
#include "update.c"
#include "delete.c"
#include "save.c"
#include "string.c"

// import enhancement
#include "sortsummary.c"

// import unique feature (if any)
#include "grade.c"


