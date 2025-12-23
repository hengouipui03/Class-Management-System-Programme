#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//hash function to specify which slot should a particular
//student data go into
unsigned int hash (int keyId){
    unsigned int hash_value = keyId * keyId; //this is how we will calculate our unique hash value
    return hash_value % STUDENT_SIZE; //then we mod it to ensure the chosen slot stays within the table size
}

// here we simply initialise the table
bool initialise(){
    for(int i=0; i < STUDENT_SIZE; i++){
        hash_table[i] = NULL; //each slot is set to NULL to indicate it's empty
    }
    return true;
    // table is empty
}

//function to insert a student's data into the hash table
bool hashInsert(Student *st){
    if (st == NULL) return false; //safety check to ensure we aren’t inserting an empty record

    int index = hash(st -> keyID); //here we calculate which slot the student data should go into

    //linear probing to handle collisions
    for (int i = 0; i < STUDENT_SIZE; i++){
        int next = (i + index) % STUDENT_SIZE; //if the calculated slot is taken, move to the next one in a circular fashion

        //if the slot is empty or has a deleted record, we can use it
        if(hash_table[next] == NULL || hash_table[next] == DELETED_NODE){
            hash_table[next] = st; //store the student’s data in this slot
            return true; //successful insertion
        }
    }

    return false; //if no free slot is found after checking all, return false
}

static int hashSlot(int keyID) {  // hashSlot function to be called for Lookup, Delete & Update: less repeat code
    int index = hash(keyID);

    for (int i = 0; i < STUDENT_SIZE; i++){
        int next = (index + i) % STUDENT_SIZE;

        if(hash_table[next] == NULL){
            return -1;
        }

        if(hash_table[next] == DELETED_NODE) continue;

        if (hash_table[next] != NULL && hash_table[next]->keyID == keyID) {
            return next;
        }
    }
    return -1; 
}

//function to find a student's record in the table using their keyID
Student *hashLookup(int keyID) {
    int index = hashSlot(keyID); //calculate the index where the record should be

    if (index == -1) {
        return NULL;
    }
    return hash_table[index];
}

//function to delete a student record using their keyID
Student *hashDelete(int keyID){
    int index = hashSlot(keyID);

    if (index == -1) {
        return NULL;
    } 
    Student *deleted_Student = hash_table[index]; //store the student data temporarily
    hash_table[index] = DELETED_NODE; //mark the slot as deleted
    return deleted_Student; //return the deleted student record
}

bool hashUpdate(int keyID, float *new_mark, char *new_prog, char *new_name) {
    int index = hashSlot(keyID);
    
    if (index == -1) {
        return false; // student is not found
    }
    
    Student *student = hash_table[index];
    
    // update mark if provided by admin
    if (new_mark != NULL) {
        student->mark = *new_mark;
        // re-assign grade based on updated mark
        assignGrade(student);
    }
    
    // update programme if provided by admin
    if (new_prog != NULL) {
        strncpy(student->programme, new_prog, sizeof(student->programme) - 1);
    }

    if (new_name != NULL) {
        strncpy(student->name, new_name, sizeof(student->name) - 1);
    }
    
    return true;
}

// validation: empty database / hashtable
int count(void){
    int count = 0;  // set count variable to 0
    for (int i = 0; i < STUDENT_SIZE; i++) {    // iterate through all slots (size of hashtable:student_size) in the hashtable
        if (hash_table[i] != NULL && hash_table[i] != DELETED_NODE) {   // if hashtable slot is not empty & not deleted (exclude both empty and deleted)
            count++;    // increase the count for slots that contain the no. of active student records
        }
    }
return count;   // return the no. of active student records
}
