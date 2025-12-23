#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float newMark;
char newProg[200]; 
char newName[200];

float *markPtr = NULL;
char *progPtr = NULL;
char *namePtr = NULL;

// extract categories in admin input
int extractCategory(char *input, const char *categoryName, void *result, int isNumeric) {
    char buffer[200];

    // search for any category in admin input, if found pointer will move to it
    char *categoryPtr = strstr(input, categoryName); 
    if (categoryPtr == NULL) {
        return 0; // else, category is not found; dont update this field 
    }
    
    // skip the category name (e.g., "Mark=", "Name=")
    char *valueStart = categoryPtr + strlen(categoryName);
    
    // find the end of this category's input; stop at the next category or end of current input
    char *valueEnd = valueStart;
    
    // find all possible categories from admin input via parsing using strstr
    // strstr: finds first occurence of substring in string & returns a pointer - <string.h>
    char *nextID = strstr(valueStart, "ID=");
    char *nextName = strstr(valueStart, "Name=");
    char *nextProg = strstr(valueStart, "Programme=");
    char *nextMark = strstr(valueStart, "Mark=");
    
    // find the closest next category marker
    valueEnd = valueStart + strlen(valueStart); // bring to the end of admin input
    

    // finds any category from admin input in any order
    // validation: all orders of student update categories input from admin
    if (nextID && nextID < valueEnd) valueEnd = nextID;
    if (nextName && nextName < valueEnd) valueEnd = nextName;
    if (nextProg && nextProg < valueEnd) valueEnd = nextProg;
    if (nextMark && nextMark < valueEnd) valueEnd = nextMark;
    
    // calculate length and copy
    int len = valueEnd - valueStart;
    if (len >= 200) {
        len = 199;  // prevent buffer overflow
    }
    
    strncpy(buffer, valueStart, len);
    buffer[len] = '\0';
    
    // remove trailing tabs in between categories by admin input
    while (len > 0 && (buffer[len-1] == ' ' || buffer[len-1] == '\t')) {
        buffer[--len] = '\0';
    }
    
    if (len == 0) return 0;
    
    // numeric float conversion for mark
    if (isNumeric) {
        char *endPtr;  
        float *floatResult = (float *)result;

        // strtof: string to float
        *floatResult = strtof(buffer, &endPtr);
        
        // check conversion of string to float and return -1 for invalid format
        if (endPtr == buffer || *endPtr != '\0') {
            return -1; // validation: letters (abc), mulitple decimal place (54..4) and special char (!#~)
        }
        return 1; // else, return value of valid float
    } 
    // string copy
    else {
        strcpy((char *)result, buffer);
        return 1; // category found and is extracted for update function
    }
}
    // return: 1 = category found, 0 = category not found, -1 = invalid format (for mark)

// function to find duplicate    
int duplicateCategory(char *input, const char *categoryName) {
    char *first = strstr(input, categoryName);     // find first occurance of category in admin input
    if (first) {
        char *second = strstr(first + strlen(categoryName), categoryName);  // find second repeat occurance in admin input
        if (second) {   
            return 1; // return true; duplicate found
        }
    }
    return 0; // else, no duplicate
}
int promptAndCheckCategory(char *categoryName, char *categoryPresent, void *categoryPtr) {
    char check[10];
    while (true)
    {
        if (categoryPtr == NULL && categoryPresent != NULL) {
            printf("CMS: The '%s' category is empty. Would you like to continue without updating it? (Y/N): ", categoryName);
            printf("Type \"Y\" to Confirm or type \"N\" to cancel.\n"
                    "P7_1: ");

            fgets(check, sizeof(check), stdin);
            check[strcspn(check, "\n")] = 0;  // remove new line at the end when using fgets()

            if (strcmp(check, "N") == 0) {
                printf("CMS: The update is cancelled.\n");    
                return 0;

            // N detected: cancel delete action
            } else if (strcmp(check, "Y") == 0) {
                return 1;                
            } else {
                // validation: invalid input from admin - prompt admin until input follows Appendix A
                printf("CMS: Invalid input. Please type 'Y' or 'N'.\n");
            }

        }
        else {
            return 1;
        }
    }
}

void update(char *input) {
    // validation: remove spaces around = from admin input
    str_clean(input);

    markPtr = NULL;
    progPtr = NULL;
    namePtr = NULL;
    
    int digit = 0;
    if (count() == 0) {     // check hashtable for active student records stored
        // validation: no student records in database
        printf("CMS: There are no student records to update.\n");
        return;
    }

    char *markCategory = strstr(input, "Mark=");
    char *progCategory = strstr(input, "Programme=");
    char *nameCategory = strstr(input, "Name=");

    // validation: category input is not given by admin properly
    if (strstr(input, " Mark") != NULL && markCategory == NULL) {
        printf("CMS: Invalid input. Category:'Mark' has no value. Please type 'Mark=[StudentMark]'.\n");
        return;
    }
    if (strstr(input, " Programme") != NULL && progCategory == NULL) {
        printf("CMS: Invalid input. Category:'Programme' has no input. Please type 'Programme=[StudentProgramme]'.\n");
        return;
    }
    if (strstr(input, " Name") != NULL && nameCategory == NULL) {
        printf("CMS: Invalid input. Category:'Name' has no input. Please type 'Name=[StudentName]'\n");
        return;
    }

    // validation: search for duplicate categories from admin input
    if (duplicateCategory(input, "Mark=")) {
        printf("CMS: Invalid input. Category:'Mark' appears multiple times. Please type only once.\n");
        return;
    }
    
    if (duplicateCategory(input, "Programme=")) {
        printf("CMS: Invalid input. Category:'Programme' appears multiple times. Please type only once.\n");
        return;
    }
    
    if (duplicateCategory(input, "Name=")) {
        printf("CMS: Invalid input. Category:'Name' appears multiple times. Please type only once.\n");
        return;
    }
    
    if (duplicateCategory(input, "ID=")) {
        printf("CMS: Invalid input. Category:'ID' appears multiple times. Please type only once.\n");
        return;
    }

    // parse using strstr and check if "ID=" exists in admin input 
    // validation: display missing "ID=" error statement and prompt admin to input "ID"
    char *ptrid = strstr(input, "ID=");
    if (!ptrid) {
        printf("CMS: Missing \"ID=\". Please type: \"UPDATE ID=\"\n");
        return;
    }

    // validation: display missing student ID and prompt admin to input a student ID
    char idstr[20];
    if (extractCategory(input, "ID=", idstr, 0) == 0) {
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

    // find "Mark=" from admin input
    int markResult = extractCategory(input, "Mark=", &newMark, 1);
    if (markResult != 0) {
        if (markResult == -1) {
            // validaton: student mark cannot be special characters, letters or have multiple decimal place
            printf("CMS: Invalid Mark format. Mark must be a number.\n");
            return;
        }
        // validation: mark range must be between 0 and 100
        if (newMark < 0 || newMark > 100) {
            printf("CMS: Invalid Mark. Mark must be between 0 and 100.\n");
            return;
        }
        
        markPtr = &newMark;
    }

    // find "Programme=" from admin input
    char progTemp[200];
    if (extractCategory(input, "Programme=", progTemp, 0) == 1) { 
        if (strlen(progTemp) == 0) {
            // valiation: student programme must not be updated as empty in hashtable
            printf("CMS: Student Programme cannot be empty.\n");
            return;
        }
        // validation: student programme only contains letters and space using isalpha
        for (int i = 0; progTemp[i]; i++) {
            if (!isalpha((unsigned char)progTemp[i]) && progTemp[i] != ' ') {
                printf("CMS: Invalid Programme. Programme can only contain letters and spaces.\n");
                return; 
            }
        }
        strcpy(newProg, progTemp);
        progPtr = newProg;
    }

    // find "Name=" from admin input
    char nameTemp[200];
    int updateMark, updateProg, updateName = 0;

    if (extractCategory(input, "Name=", nameTemp, 0) == 1) { // 0 = string
        if (strlen(nameTemp) == 0) {
            // validation: student name must not be updated as empty in hashtable
            printf("CMS: Student Name cannot be empty.\n"); 
            return;
        }
        // validation: student name cannot have digits or special characters using isalpha
        for (int i = 0; nameTemp[i]; i++) {
            if (!isalpha((unsigned char)nameTemp[i]) && nameTemp[i] != ' ') {
                printf("CMS: Invalid Name. Name can only contain letters and spaces.\n");
                return;
            }
        }
        strcpy(newName, nameTemp);
        namePtr = newName;
    }

    // validation: check if at least one category was provided; prompt admin to input with statement
    if (markPtr == NULL && progPtr == NULL && namePtr == NULL) {
        printf("CMS: No category to update. Provide type any of the following: Name=[StudentName], Programme=[StudentProgramme], or Mark=[StudentMark]\n");
        return;
    }
    updateMark=promptAndCheckCategory("Mark", markCategory, markPtr);  
    updateProg=promptAndCheckCategory("Programme", progCategory, progPtr);  
    updateName=promptAndCheckCategory("Name",nameCategory, namePtr);  
    if (updateMark && updateProg && updateName) {
        if (hashUpdate(id, markPtr, progPtr, namePtr)){      
        printf("CMS: The record with ID=%d is successfully updated.\n", id);    // sample response: Appendix A
        }
        else {
            printf("CMS: The record with ID=%d does not exist.\n", id);   // sample response: Appendix Aprintf("CMS: Failed to update record with ID=%d.\n", id);
        }   
    } else {
        printf("CMS: Failed to update record with ID=%d.\n", id);
    }

}    

