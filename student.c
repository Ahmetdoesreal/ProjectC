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

//This Project is multiple files using #define makes it easier to change values
#define name (64+1)             //defines Max Name Length  
#define maxOffice 20            //defines Maximum Office Hours per Teaecher

//Structure to contain teacher and office hour information
struct TeacherInfo{         //struct tag to use calloc(thank you microsoft >:[ )
    char Name[name];        //Teacher Name
    struct OfficeHourInfo{  //office hour information
    	int start, end;     //starting and ending hour variables
    	enum { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;
        //Enumeration to keep track of day (makes it easier to identify dates)
    	enum { DNE, Taken, NotTaken }status;
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
    	fprintf(stderr, "Appointment file opening failed\nAttempting to create one");
        //error message
    	//FILE* help;
        ;
        /*
    	appointment = fopen("appointment.txt", "w+");
    	if (NULL != appointment) {*/
            // if(appointment==NULL){
            //     printf("NULL");
            // }else
            // printf("%Ld",appointment);

    	if ((appointment=fopen("appointment.txt", "w+"))!=NULL){
    	    fprintf(appointment, "Teacher\t\tStudent\t\tDay\t\tStart\tEnd\n");		
    	}
    	else {
    	    fprintf(stderr, "Error creating appointment file");
    	    return;
    	}
    }
    if(appointment!=NULL){	
	fseek(appointment, 0L, SEEK_END);
	printf("\nlogged in as %s\nsearching for %s\n", studentName, teacherName);
	FILE* file;
	char filename[name + 4];
	strcpy(filename, teacherName);
	strcat(filename, ".dat");
	if (NULL != (file = fopen(filename, "rb+"))) {
	    print_office_hour(teacherName, true);
	    struct TeacherInfo info;
	    fread(&info, sizeof(struct TeacherInfo), 1, file);
	    printf("\tEnter Desired identification number of office hour: ");
	    int id = -1;
	    scanf("%d", &id);
	    if (id >= maxOffice || id < 0) {
	        fprintf(stderr, "Illegal Office Hour ID must be in range[%d-%d]", 0, maxOffice);
	    }
	    else if (info.ID[id].status != NotTaken)
	        fprintf(stderr, "This office hour is unavalibe");
	    else {
	        strcpy(info.ID[id].takenby, studentName);
	        info.ID[id].status = Taken;
	        fprintf(appointment,"%-15.15s %-15.15s ",teacherName,studentName);
	        switch (info.ID[id].day){
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
	        	fprintf(stderr, "An Error Occured in Date Printing" __FILE__);
	        	break;
	        }
	        fprintf(appointment,"\t");
	        if (info.ID[id].start < 12)
	        	fprintf(appointment,"%d a.m.", ((info.ID[id].start + 11) % 12) + 1);
	        else
	        	fprintf(appointment,"%d p.m.", ((info.ID[id].start + 11) % 12) + 1);
	        fprintf(appointment,"\t");
	        if (info.ID[id].end < 12)
	        	fprintf(appointment,"%d a.m.", ((info.ID[id].end + 11) % 12) + 1);
	        else
	        	fprintf(appointment,"%d p.m.", ((info.ID[id].end + 11) % 12) + 1);
	        fprintf(appointment,"\n");
	        printf("%-15.15s %-15.15s ", teacherName, studentName);
	        rewind(file);
	        fwrite(&info, sizeof(struct TeacherInfo), 1, file);
	        fclose(file);
	    }
        }
    else
	printf("Couldn't find Teacher %s Please refine your input.", teacherName);
    }
}