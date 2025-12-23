#ifndef STRING_C   
#define STRING_C 
#include <stdio.h>

void str_clean(char *input) {
    int i = 0;
    int j = 0;
    // formatting in the event there are spaces in places where there should not be
    while (input[i] != '\0') {
        // Case 1: space before '='
        if (input[i] == ' ' && input[i + 1] == '=') {
            i++;  // skip this space
            continue; // go to next character
        }
        // Case 2: space after '='
        if (input[i] == '=' && input[i + 1] == ' ') {
            input[j++] = input[i++]; // copy '=' first
            while (input[i] == ' ') i++; // skip all spaces after '='
            continue; // continue with next character
        }
        // Normal character, just copy it
        input[j++] = input[i++];
    }
    input[j] = '\0'; // null the cleaned string
}

#endif 