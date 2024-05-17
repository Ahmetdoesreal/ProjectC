#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "student.h"
#include "teacher.h"
#define name (64+1)
#define maxOffice 10
struct TeacherInfo
{
	char Name[name];
	struct
	{
		int start, end;
		enum { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;
		enum { DNE, Taken, NotTaken }status;
		char takenby[name];
	}ID[maxOffice];
}; 
void create_appointment(char* studentName, char* teacherName) {
	FILE* appointment;
	if (NULL == (appointment = fopen("appointment.txt", "r+"))) {
		fprintf(stderr, "Appointment file opening failed\nAttempting to create one");
		FILE* help;/*
		appointment = fopen("appointment.txt", "w+");
		if (NULL != appointment) {*/
		if ((help = fopen("appointment.txt", "w+"))!=NULL){
			appointment = help;
			fprintf(appointment, "Teacher\t\t\tStudent\t\t\tDay\t\t\t\tStart\tEnd\n");
			fprintf(stdout, "Teacher\t\t\tStudent\t\t\tDay\t\t\t\tStart\tEnd\n");
		
		}
		else {
			fprintf(stderr, "Error creating appointment file");
			return;
		}
	}
	if(appointment!=NULL)
	{	
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
				switch (info.ID[id].day)
				{
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
				fprintf(appointment,"\t\t");
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
				switch (info.ID[id].day)
				{
				case Monday:
					printf("Monday\t");
					break;
				case Tuesday:
					printf("Tuesday\t");
					break;
				case Wednesday:
					printf("Wednesday");
					break;
				case Thursday:
					printf("Thursday");
					break;
				case Friday:
					printf("Friday\t");
					break;
				case Saturday:
					printf("Saturday");
					break;
				case Sunday:
					printf("Sunday\t");
					break;
				default:
					fprintf(stderr, "An Error Occured in Date Printing" __FILE__);
					break;
				}
				printf("\t");
				if (info.ID[id].start < 12)
					printf("%d a.m.", ((info.ID[id].start + 11) % 12) + 1);
				else
					printf("%d p.m.", ((info.ID[id].start + 11) % 12) + 1);
				printf("\t");
				if (info.ID[id].end < 12)
					printf("%d a.m.", ((info.ID[id].end + 11) % 12) + 1);
				else
					printf("%d p.m.", ((info.ID[id].end + 11) % 12) + 1);
				printf("\n");
				rewind(file);
				fwrite(&info, sizeof(struct TeacherInfo), 1, file);
				fclose(file);
			}
		}
		else
			printf("Couldn't find Teacher %s Please refine your input.", teacherName);

	}
}