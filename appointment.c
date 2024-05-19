/*
appointment.c

2300005633
Ahmet Deniz Sezgin
Office Hour Appointment System

The program tracks the appointment of office hours

*/
#define _CRT_SECURE_NO_WARNINGS //makes it so Microsoft C Runtime Compiler doesnt complain about input functions
#include <stdio.h>              //Standard C Input Output Header
#include <string.h>             //Standard String Header
#include <stdlib.h>             //Standard Library Header
#include "readline.h"           //Readline function header
#include "teacher.h"            //Teacher functions header that contains prototypes
#include "student.h"            //Student functions header that contains prototypes
//uses predefined macros to print info This file's name.extension,Compile Date,Compile time,Line Wherever this macro is called
#define ProjectInfo printf("The project file information is given below.\n" \
				"\tFile name:" __FILE__ "\n"\
				"\tDate:" __DATE__ "\n"\
				"\tTime:" __TIME__ "\n"\
				"\tLine:%d\n\n",__LINE__)       
//prints out requested file information
//This Project is multiple files using #define makes it easier to change values

#define name (32+1)             //defines Max Name Length  
#define maxOffice 20            //defines Maximum Office Hours per Teacher  

//Structure to contain teacher and office hour information
struct TeacherInfo{         //struct tag to use calloc(thank you microsoft >:[ )
    char Name[name];        //Teacher Name
    struct OfficeHourInfo{  //office hour information             
    	int start, end;     //starting and ending hour variables
    	enum { Monday=1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;//monday 1, tuesday 2,....sunday 7
        //Enumeration to keep track of day (makes it easier to identify dates)
    	enum{DNE,Taken,NotTaken}status;//DNE 0, Taken 1, Not taken 2;
        //Enumaration to keep track of status of the hour (makes it easier to create conditions)
    	char takenby[name];
        //If taken by a student stores the students name inside the teachers file
    }ID[maxOffice];
    //instead of giving ID's to each structure the array's elements are the id's itself
};

int main(void) {//main function
    char Selection = 0;//variable to store selection information
    ProjectInfo;//calling macro for requested info
    printf( "**************************************************\n"\
            "* WELCOME TO THE OFFICE HOUR APPOINTMENT SYSTEM  *\n"\
            "**************************************************\n");
            //Welcome Message nothing special
    //a loop (will explain what it does inside)
    do {
    	char input[name] = "";//input for name string
        printf("Who are you? (Enter 1 for TEACHER, 2 for STUDENT,3 to Exit):");
        //prompt user for use case
    	read_line(&Selection, 1);
        //gets input
    	if (Selection == '1') {//       If user is a teacher
    	    printf("Enter your full name: ");//asks for name
    	    read_line(input, name);//gets the name

            //data files are stored as .dat files in examples.
    	    char test[name + 4];//NOT TO BE CONFUSED THIS IS NOT AN EXPERIMENTAL VARIABLE NAME TEST HERE STANDS FOR TESTING IF THE USER DATA EXISTS
    	    strcpy(test, input);
    	    strcat(test, ".dat");


    	    FILE* testf;//test file pointer
    	    if ((testf = fopen(test, "rb")) == NULL) {//if file open is failede
    	    	fprintf(stderr,"Could not find information file for Teacher %s as file \"%s\"\nWould You like to create file \"%s\" (Y/N)", input, test, test);
                //shows the error message it cant find information for teacher than asks user should the program create a data file 
    	    	read_line(&Selection, 1);//gets input
    	    	if (Selection == 'Y' || Selection == 'y') {         //if answer is yes
    	    	    if ((testf = fopen(test, "wb+")) == NULL) {     //tries to create file if fails
    	    	    	fprintf(stderr, "File Creation failed");    //shows error message then returns with value -1 
    	    	    	return -1;//return -1 to indicate failed termination, if it cannot even create the data file it cannot work with data files,just a precation
    	    	    }
    	    	    else {//if file creation is succesfull
    	    	    	struct TeacherInfo *newteach;//creates a pointer to a teacher structure (i was gonna just declare a regular variable but microsoft decides to fill it with C's instead of 0's)
    	    	    	newteach = calloc(sizeof(struct TeacherInfo), 1);//calloc allocates memory for teacher information and sets all the bits to 0 and sets the result pointer to the teacher pointer
    	    	    	strcpy(newteach->Name, input);//teacher name stored in the teacherinfo structure
    	    	    	rewind(testf);//just in case the cursor is not at the start
    	    	    	fwrite(newteach, sizeof(struct TeacherInfo), 1, testf);//writes teacher structure information to the file
    	    	    	fclose(testf);//closes testing file but in this case it is new teacher file
    	    	    }
    	    	}
    	    	else
                    continue;
                    //If user doesn't want to create new data file
                    //maybe they had a mistake while input
                    //goes to the end of the loop
    	    }
    	    printf("Welcome %s", input);//Welcomes user
    	    do {
    	    	printf("\nSelect operation Insert,Update,Print,Quit ");//prompts user for an operation
    	    	read_line(&Selection, 1);//gets input 
    	    	switch (Selection)//compares input
    	    	{
    	    	case 'i':case 'I'://if insert selected
    	    		insert_office_hour(input);// calls insert function
    	    		break;//break used to make cases independent of each other 
    	    	case 'u':case 'U'://if update selected
    	    		update_office_hour(input, NULL);// calls update function
    	    		break;
    	    	case 'p':case'P'://if print selected
    	    		print_office_hour(input,false);// calss print function
    	    		break;
    	    	case 'q':case 'Q':case'\n'://if quit selected does nothing 
    	    		break;
    	    	default://if user fails to enter 
    	    		fprintf(stderr, "\a\nPlease select a supported action\n[i,I]\tInsert\n[u,U]\tUpdate\n[p,P]\tPrint\n[q,Q]\tQuit");
                        //shows error message to tell user they did not manage to select from the few options they had
    	    		break;
    	    	}
    	    } while (Selection != 'q' && Selection != 'Q');//continues until the selection is Q or q
    	}
    	else if (Selection == '2') {//if user is student
    	    printf("Enter your full name: ");//asks for name
    	    read_line(input, name);//gets input
    	    printf("Welcome %s", input);//Welcomes The User
    	    do {
    	    	char teach[name] = "";//initializes teacher name string
    	    	printf("\nEnter Teacher name:");//prompts for teacher name
    	    	read_line(teach, name);//gets input
    	    	Selection = teach[0];//sets selection to teachers first letter
    	    	if (teach[0] != 'q' && teach[0] != 'Q')//if teachers first letter is not q or Q
    	    	    create_appointment(input, teach);//calls create appointment function
    	    } while (Selection != 'q' && Selection != 'Q');// i could use the if(condition){statement}else break; to exit the loop but i used this at the previous loop
    	}
    	else if(Selection!='3') //if the first selection is not 1 or 2 or 3 
    	    fprintf(stderr, "Please enter 1 or 2 or 3(Exit)\a");//prints out a much more  basic error message stating they have 3 options
    } while (Selection != '3');//if the selection is not 3 continues the loop
    return 0;//return 0 successfull termination
}