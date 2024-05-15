#include <stdio.h>
#include "teacher.h"
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
		char takenby[name];
	}ID[maxOffice];
};
struct TeacherInfo info;
FILE *file;

void store_offce_hour(void){
rewind(file);
fwrite(info,sizeof(info),1,file);
fclose(file);
}
void insert_office_hour(void) {
	printf("insert selected\n");
}
void update_office_hour(void) {
	printf("update selected\n");
}
void print_office_hour(char*tName) {
	char filename[name+4]="";
	printf("print selected\n");
	strcpy(filename,tName);
	strcat(filename,".dat");
	if(file=fopen(""))

}