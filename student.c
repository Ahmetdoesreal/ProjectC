/*
student.c

2300005633
Ahmet Deniz Sezgin
Student Functions and appointment file operations

*/
#define _CRT_SECURE_NO_WARNINGS //makes it so Microsoft C Runtime Compiler doesnt complain about input functions
#include <stdio.h>              //Standard C Input Output library
#include <string.h>             //Standard String Operations library
#include "student.h"            //Header file that contains Student Functions Prototypes
#include "teacher.h"            //Header file that contains Teacher Functions Prototypes
#include "readline.h"           //Header file for readline function

//This Project is multiple files using #define makes it easier to change values
#define name (32+1)             //defines Max Name Length  
#define maxOffice 20            //defines Maximum Office Hours per Teacher

//Structure to contain teacher and office hour information
struct TeacherInfo{         //struct tag to use calloc(thank you microsoft >:[ )
    char Name[name];        //Teacher Name
    struct OfficeHourInfo{  //office hour information
    	int start, end;     //starting and ending hour variables
    	enum { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;//monday 1,tuesday 2....sunday 7
        //Enumeration to keep track of day (makes it easier to identify dates)
    	enum { DNE, Taken, NotTaken }status;//dne 0,taken 1, not taken 2
        //Enumaration to keep track of status of the hour (makes it easier to create conditions)
    	char takenby[name];
        //If taken by a student stores the students name inside the teachers file
    }ID[maxOffice];
    //instead of giving ID's to each structure the array's elements are the id's itself
}; 

//create appointment function
void create_appointment(char* studentName, char* teacherName) {
    FILE* appointment;//appointment file pointer declaration
    if (NULL == (appointment = fopen("appointment.txt", "r+"))) {//checks to see if appointment file can be opened
    	fprintf(stderr, "Appointment file opening failed\nAttempting to create one\n");
        //prints error message and informs user that the program will attempt to create a file
        
        
        //LEGACY CODE THAT WAS A FIX IN MS COMPILER

        //FILE* help;
        /*
    	appointment = fopen("appointment.txt", "w+");
    	if (NULL != appointment) {*/
            // if(appointment==NULL){
            //     printf("NULL");
            // }else
            // printf("%Ld",appointment);



    	if ((appointment=fopen("appointment.txt", "w+"))!=NULL){//creates appointment file
    	    fprintf(appointment, "Teacher\t\t\tStudent\t\t\tDay\t\tStart\tEnd\n");//writes the tables first line
    	}
    	else {//if fails to create appointment file
    	    fprintf(stderr, "Error creating appointment file\n");//shows eror message and exits the function
    	    return;
    	}
    }
    if(appointment!=NULL){	//if the file exists
	fseek(appointment, 0L, SEEK_END);//goes to the end of the file
	printf("\nLogged in as %s\nSearching for %s\n", studentName, teacherName);//prints out teacher and student info
	FILE* file;//file pointer for opening data

	char filename[name + 4];
	strcpy(filename, teacherName);
	strcat(filename, ".dat");
        //statemes that adds .dat to the end of the string (remember files are stored in *.dat format)


	if (NULL != (file = fopen(filename, "rb+"))) {//attempts to open the data files
	    print_office_hour(teacherName, true);//calls print office hour function with isStudent set as true
	    struct TeacherInfo info;                            //creates a struct variable
	    fread(&info, sizeof(struct TeacherInfo), 1, file);  //and fills the variable with data stored inside the file
	    printf("\tEnter Desired identification number of office hour: ");//prompts user to enter id of hour
	    int id = -1;//initializes id as -1
            (void)scanf("%d",&id);//gets input from user (void) is there to keep Visual Studio warning list empty(warnings annoy me)
	    if (id >= maxOffice || id < 0) {//if id is not in range
                                            //shows error message that it is out of range
	        fprintf(stderr, "Illegal Office Hour ID must be in range[%d-%d]", 0, maxOffice-1);
	    }
	    else if (info.ID[id].status != NotTaken)//if the office hour is taken or does not exist
	        fprintf(stderr, "This office hour is unavalibe");// tells user that they cant claim that office hour
	    else {//if the office hour is not taken (also exists)
	        strcpy(info.ID[id].takenby, studentName);//copies logged in student name to takenby of structre
	        info.ID[id].status = Taken;//sets status to taken
	        fprintf(appointment,"%-23.23s %-23.23s ",teacherName,studentName);//prints out teacher name and student name to the appointmnet file
	        switch (info.ID[id].day){//checks for day and prints out the day data to the appointment file
	        case Monday:
	        	fprintf(appointment,"Monday\t");
	        	break;
	        case Tuesday:
	        	fprintf(appointment,"Tuesday\t");
	        	break;
	        case Wednesday:
	        	fprintf(appointment,"Wednesday");
	        	break;
	        case Thursday:
	        	fprintf(appointment,"Thursday");
	        	break;
	        case Friday:
	        	fprintf(appointment,"Friday\t");
	        	break;
	        case Saturday:
	        	fprintf(appointment,"Saturday");
	        	break;
	        case Sunday:
	        	fprintf(appointment,"Sunday\t");
	        	break;
	        default:
	        	fprintf(stderr, "An Error Occured in Date Printing" __FILE__);//error report message, there is more than 1 date print function so __FILE__ helps pretty much
	        	break;
	        }
	        fprintf(appointment,"\t");//just a tab
	        if (info.ID[id].start < 12)//if the start hour value is below 12
	        	fprintf(appointment,"%d a.m.", ((info.ID[id].start + 11) % 12) + 1);//prints the converted value 24 hr to 12 hr format
	        else//if is not below 12
	        	fprintf(appointment,"%d p.m.", ((info.ID[id].start + 11) % 12) + 1);//prints the converted value 24 hr to 12 hr format
	        fprintf(appointment,"\t");//    tab
	        if (info.ID[id].end < 12)//if the end hour value is below 12
	        	fprintf(appointment,"%d a.m.", ((info.ID[id].end + 11) % 12) + 1);//prints the converted value 24 hr to 12 hr format
	        else//if it is not below 12
	        	fprintf(appointment,"%d p.m.", ((info.ID[id].end + 11) % 12) + 1);//prints the converted value 24 hr to 12 hr format
	        fprintf(appointment,"\n");//newline
	        rewind(file);//goes to the start of the file
	        fwrite(&info, sizeof(struct TeacherInfo), 1, file);//saves info
	        fclose(file);//closes file
	    }
        }
    else
	fprintf(stderr,"Couldn't find Teacher %s Please refine your input.\n", teacherName);//shows error message that teacher is not found
    }
}