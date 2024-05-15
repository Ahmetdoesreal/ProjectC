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
#define flag int
#define true 1
#define false 0

struct TeacherInfo
{
	char Name[name];
	struct
	{
		int id,start, end;
		enum { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;
		flag taken;
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
	strcpy(x.Name, "Ahmet Deniz Sezgin");
	x.ID[0].id = 1;
	x.ID[0].day = Monday;
	x.ID[0].start = 13;
	x.ID[0].end = 14;
	x.ID[0].taken =false;
	FILE* fp,*fp2;
	char filename[name + 3];
	strcpy(filename, x.Name);
	strcat(filename, ".dat");
	if ((fp = fopen(filename, "wb+")) != NULL) {
		printf("createfile succ");
		fwrite(&x, sizeof(x), 1, fp);
	}
	else
		printf("no");
	struct TeacherInfo patates;
	if ((fp2 = fopen("akiki.dat", "rb+")) != NULL) {
		printf("readfile succ");
		fread(&patates, sizeof(x), 1, fp2);
	}
	else
		printf("sad");
	printf("%s\n%d\n%d", patates.Name, patates.ID[0].id, patates.ID[0].taken);
}