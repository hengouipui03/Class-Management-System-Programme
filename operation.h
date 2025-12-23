#ifndef OPERATION_H
#define OPERATION_H

#include <stdbool.h>

/* table size: keep a prime for better distribution */
#define STUDENT_SIZE 31
#define DELETED_NODE (Student*)(0xFFFFFFFFFFFFFFFUL)

// struct for student
typedef struct Student {
    int keyID;
    char name[50];
    char programme[200];
    float mark;
    char grade[3];
} Student;

// external global hash table declaration
extern Student *hash_table[STUDENT_SIZE];
extern int maxID;
extern int maxName;
extern int maxProgramme;
extern int maxMark;

// initialize the table: must be called before other operations functions
bool initialise();

bool hashInsert(Student *st);


int count(void);
Student *hashDelete(int keyID);
Student *hashLookup(int keyID);
bool hashUpdate(int keyID, float *new_mark, char *new_prog, char *new_name);
void print_table(void);

// -- function declarations --
// core Operations (45%)
int openDatabase(void);
void showAll(void);
int insert(char* input);
int query(char* input);
void update(char* input);
void delete(char* input);
void save(void);
void str_clean(char *input);

// enhancement Features (10%)
int sort(char *cmpby);
void summary(void);

// unique Feature (if any)
void assignGrade(Student *st);
#endif 
