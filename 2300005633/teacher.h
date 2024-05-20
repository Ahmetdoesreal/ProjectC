/*
teacher.h

2300005633
Ahmet Deniz Sezgin
Header file for teacher functions
Prototypes for functions

*/
#ifndef TEACHER_H//if teacher_h is not defined
#define TEACHER_H//define it
#include <stdbool.h>//Standard Boolean library
void insert_office_hour(char*);//Inserts office hour to teacher file , (Teacher Name)
void update_office_hour(char*,int*);//Updates office hour of teacher file ,(Teacher Name, Pointer to ID)
void print_office_hour(char*,bool);//Prints out Office hours from teacher file, (Teacher Name,true=student,false=teacher)
#endif //end of if statement
