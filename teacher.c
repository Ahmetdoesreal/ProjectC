#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "teacher.h"
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
struct TeacherInfo info;
FILE *file;

void store_offce_hour(void){
rewind(file);
fwrite(&info,sizeof(info),1,file);
fclose(file);
}
void insert_office_hour(void) {
	printf("insert selected\n");
}
void update_office_hour(void) {
	printf("update selected\n");
}
void print_office_hour(char *tName) {
	FILE *file;
	char filename[name+4]="";
	strcpy(filename, tName);
	strcat(filename,".dat");
	if(NULL==(file=fopen(filename,"rb+")))
		fprintf(stderr,"An Error Occured While Opening File\nFunciton:Print Office Hour" __FILE__);
	else{
		fread(&info,sizeof(info),1,file);
		printf("\tID number\tDay\t\tStart\t\tEnd\n");
		for (int i = 0; i < maxOffice; i++)
		{
			if(info.ID[i].status==DNE)
				break;
			else{
				printf("\t%d\t\t",i);
				switch (info.ID[i].day)
				{
					case 0:
					printf("Monday\t");
					break;
					case 1:
					printf("Tuesday\t");
					break;
					case 2:
					printf("Wednesday");
					break;
					case 3:
					printf("Thursday\t");
					break;
					case 4:
					printf("Friday\t");
					break;
					case 5:
					printf("Saturday");
					break;
					case 6:
					printf("Sunday\t");
					break;
				default:
				fprintf(stderr,"An Error Occured in Date Printing" __FILE__);
					break;
				}
				printf("\t");
				if(info.ID[i].start<12)
					printf("%d a.m.",((info.ID[i].start+11)%12)+1);
				else
					printf("%d p.m.",((info.ID[i].start+11)%12)+1);
				printf("\t\t");
				if(info.ID[i].end<12)
					printf("%d a.m.",((info.ID[i].end+11)%12)+1);
				else
					printf("%d p.m.",((info.ID[i].end+11)%12)+1);
				printf("\n");
			}
		}
		
	}

}