/*
appointment.c

2300005633
Ahmet Deniz Sezgin
Office Hour Appointment System

The program tracks the appointment of office hours

*/
/*

So we meet again,
Start Project 15 May 02:25
since its multifile i forgor this existed :P
added file storing and updating for teacher data(16 May 22:38)
SOO BEATIFUL IN WINDOWS THE COMPILER FILLS UNUSED SPACE WITH CC IN HEX
BUT GCC THE KING DECIDES ITS ALL ZEROES
I PREFER ZERO OVER CC(17 May 01:21)
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readline.h"
#include "teacher.h"
#include "student.h"
#define ProjectInfo printf("The project file information is given below.\n" \
				"\tFile name:" __FILE__ "\n"\
				"\tDate:" __DATE__ "\n"\
				"\tTime:" __TIME__ "\n"\
				"\tLine:%d\n\n",__LINE__)
#define name (64+1)
#define maxOffice 20

struct TeacherInfo
{
    char Name[name];
    struct
    {
    	int start, end;
    	enum { Monday=1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;
    	enum{DNE,Taken,NotTaken}status;
    	char takenby[name];
    }ID[maxOffice];
};

int main(void) {
    char Selection = 0;
    ProjectInfo;

    do {
    	char input[name] = "";
    	printf( "**************************************************\n"\
                "* WELCOME TO THE OFFICE HOUR APPOINTMENT SYSTEM  *\n"\
                "**************************************************\n"\
                "Who are you? (Enter 1 for TEACHER, 2 for STUDENT):");
    	read_line(&Selection, 1);
    	if (Selection == '1') {
    	    printf("Enter your full name:");
    	    read_line(input, name);
    	    char test[name + 4];
    	    strcpy(test, input);
    	    strcat(test, ".dat");
    	    FILE* testf;
    	    if ((testf = fopen(test, "rb")) == NULL) {
    	    	printf("Could not find information file for Teacher %s as file \"%s\"\nWould You like to create file \"%s\"", input, test, test);
    	    	read_line(&Selection, 1);
    	    	if (Selection == 'Y' || Selection == 'y') {
    	    	    if ((testf = fopen(test, "wb+")) == NULL) {
    	    	    	fprintf(stderr, "File Creation failed");
    	    	    	return -1;
    	    	    }
    	    	    else {
    	    	    	struct TeacherInfo *newteach;
    	    	    	newteach = calloc(sizeof(struct TeacherInfo), 1);
    	    	    	strcpy(newteach->Name, input);
    	    	    	rewind(testf);
    	    	    	fwrite(newteach, sizeof(struct TeacherInfo), 1, testf);
    	    	    	fclose(testf);
    	    	    }
    	    	}
    	    	else
    	    	{
    	    	    return 0;
    	    	}
    	    }
    	    printf("welcome %s", input);
    	    do {
    	    	printf("\nselect operation Insert,Update,Print,Quit");
    	    	read_line(&Selection, 1);
    	    	switch (Selection)
    	    	{
    	    	case 'i':case 'I':
    	    		insert_office_hour(input);
    	    		break;
    	    	case 'u':case 'U':
    	    		update_office_hour(input, NULL);
    	    		break;
    	    	case 'p':case'P':
    	    		print_office_hour(input,false);
    	    		break;
    	    	case 'q':case 'Q':case'\n':
    	    		break;
    	    	default:
    	    		fprintf(stderr, "\a\nPlease select a supported action\n[i,I]\tInsert\n[u,U]\tUpdate\n[p,P]\tPrint\n[q,Q]\tQuit");
    	    		break;
    	    	}
    	    } while (Selection != 'q' && Selection != 'Q');
    	}
    	else if (Selection == '2') {
    	    printf("Enter your full name:");
    	    read_line(input, name);
    	    printf("welcome %s", input);
    	    do {
    	    	char teach[name] = "";
    	    	printf("Enter Teacher name:");
    	    	read_line(teach, name);
    	    	Selection = teach[0];
    	    	if (teach[0] != 'q' && teach[0] != 'Q')
    	    	    create_appointment(input, teach);
    	    } while (Selection != 'q' && Selection != 'Q');
    	}
    	else if(Selection!='3') {
    	    fprintf(stderr, "Please enter 1 or 2\a");
    	}
    } while (Selection != '3');
    return 0;
}