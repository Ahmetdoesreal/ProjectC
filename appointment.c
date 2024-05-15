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


*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "readline.h"
#include "teacher.h"
#define ProjectInfo printf("The project file information is given below.\n" \
							"\tFile name:" __FILE__ "\n"\
							"\tDate:" __DATE__ "\n"\
							"\tTime:" __TIME__ "\n"\
							"\tLine:%d\n\n",__LINE__)
#define name (64+1)
#define maxOffice 5

struct TeacherInfo
{
	char Name[name];
	struct
	{
		int start, end;
		enum { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;
		enum{DNE,Taken,NotTaken}status;
		char takenby[name];
	}ID[maxOffice];
};
//int main(void) {
//
//	char Selection = 0;
//	char input[name]="";
//	ProjectInfo;
//	printf("**************************************************\n* WELCOME TO THE OFFICE HOUR APPOINTMENT SYSTEM  *\n**************************************************\nWho are you? (Enter 1 for TEACHER, 2 for STUDENT):");
//	read_line(&Selection, 1);
//	if (Selection=='1') {
//		printf("Enter your full name:");
//		read_line(input,name);
//		printf("welcome %s", input);
//		do{
//			printf("\nselect operation Insert,Update,Print,Quit");
//			read_line(&Selection, 1);
//			switch (Selection)
//			{
//			case 'i':case 'I':
//				insert_office_hour();
//				break;
//			case 'u':case 'U':
//				update_office_hour();
//				break;
//			case 'p':case'P':
//				print_office_hour();
//				break;
//			case 'q':case 'Q':case'\n':
//				break;
//			default:
//				fprintf(stderr, "\a\nPlease select a supported action\n[i,I]\tInsert\n[u,U]\tUpdate\n[p,P]\tPrint\n[q,Q]\tQuit");
//				break;
//			}
//		} while (Selection != 'q' && Selection != 'Q');
//	}
//	else if (Selection == '2') {
//		printf("Enter your full name:");
//		read_line(input, name);
//		printf("welcome %s", input);
//		do{
//			char teach[name] = "";
//			printf("Enter Teacher name:");
//			read_line(teach, name);
//			Selection = teach[0];
//			if(teach[0]!='q'&&teach[0]!='Q')
//			create_appointment(input,teach);
//		} while (Selection != 'q' && Selection != 'Q');
//	}
//	else {
//		fprintf(stderr, "Please enter 1 or 2\a");
//	}
//}
int main(void) {
	struct TeacherInfo x;
	x.ID[2].status=DNE;
	x.ID[3].status=DNE;
	x.ID[4].status=DNE;
	strcpy(x.Name,"Deniz Sezgin");
	x.ID[0].status=NotTaken;
	x.ID[0].day=Tuesday;
	x.ID[0].start=13;
	x.ID[0].end=14;
	x.ID[1].status=Taken;
	x.ID[1].day=Wednesday;
	x.ID[1].start=20;
	x.ID[1].end=22;
	strcpy(x.ID[1].takenby,"Ahmet Deniz");
	FILE* fp,*fp2;
	char filename[name + 3];
	strcpy(filename, x.Name);
	strcat(filename, ".dat");
	if ((fp = fopen(filename, "wb+")) != NULL)
		fwrite(&x, sizeof(x), 1, fp);
	else
		fprintf(stderr,"no");
	fclose(fp);
	struct TeacherInfo patates;
	if ((fp2 = fopen(filename, "rb+")) != NULL)
		fread(&patates, sizeof(x), 1, fp2);
	else
		fprintf(stderr,"sad");
	printf("\n");
	print_office_hour(x.Name);
}