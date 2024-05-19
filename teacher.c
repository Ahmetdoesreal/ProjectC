#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "teacher.h"
#include "readline.h"
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
struct TeacherInfo info;
FILE *file;
int find_office_hour(char* tName){
    printf("%s",tName);
    char filename[name+4]="";
    strcpy(filename,tName);
    strcat(filename,".dat");
    FILE *find;
    struct TeacherInfo findInfo;
    printf("%s",filename);
    if(NULL==(find=fopen(filename,"rb+"))){
    fprintf(stderr,"File Open failed \"%s\"\n",filename);
    return -2;
    }else{
        fread(&findInfo,sizeof(struct TeacherInfo),1,find);
        fclose(find);
        int id=-1;
        printf("Enter identification number: ");
        scanf("%d",&id);
        if (id > maxOffice || id < 0){
        fprintf(stderr,"Entered ID:%d must be in legal range [%d-%d]\n", id, 0, maxOffice);
            return -3;
        }
        else if(DNE!=findInfo.ID[id].status)
            return id;
        else{
            fprintf(stderr,"Office hour does not exist\n");
            return -1;
        }
    }
}
void store_office_hour(void){
    rewind(file);
    fwrite(&info, sizeof(info), 1, file);
    fclose(file);
}
void insert_office_hour(char* tName) {
    int idInput = -1,hourInput=-1;
    char inputInfo[10] = "";
    char filename[name + 4] = "";
    strcpy(filename, tName);
    strcat(filename, ".dat");
    if (NULL == (file = fopen(filename, "rb+"))) {
    	fprintf(stderr, "An Error Occured While Opening File \"%s\"\nFunciton:Insert Office Hour %s",filename, __FILE__);
    }
    else {
    	fread(&info, sizeof(info), 1, file);
    	printf("\tEnter identification number: ");
    	(void)scanf("%d", &idInput);
    	if (!(0<=idInput&&idInput < maxOffice))
    	    fprintf(stderr, "Illegal Office Hour ID must be in range[%d-%d]",0,maxOffice);
    	else
    	{
    	    switch (info.ID[idInput].status)
    	    {
    	    case DNE:{
    	    FailedDay:
    	    	printf("\tEnter a day: ");
    	    	read_line(inputInfo, 10);
    	    	switch (inputInfo[0])
    	    	{
    	    	case 'M':case 'm':
    	    	    info.ID[idInput].day = Monday;
    	    	    break;
    	    	case 't':case 'T':
    	    	    if (inputInfo[1] == 'h' || inputInfo[1] == 'H')
    	    	        info.ID[idInput].day = Thursday;
    	    	    else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
    	    	        info.ID[idInput].day = Tuesday;
    	    	    else {
    	    	        fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	        goto FailedDay;
    	    	    }
    	    	    break;
    	    	case 'w':case 'W':
    	    	    info.ID[idInput].day = Wednesday;
    	    	    break;
    	    	case 'F':case 'f':
    	    	    info.ID[idInput].day = Friday;
    	    	    break;
    	    	case 'S':case 's':
    	    	    if (inputInfo[1] == 'a' || inputInfo[1] == 'A')
    	    	    	info.ID[idInput].day = Saturday;
    	    	    else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
    	    	    	info.ID[idInput].day = Sunday;
    	    	    else {
    	    	    	fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	    	goto FailedDay;
    	    	    }
    	    	    break;
    	    	default:
    	    	    fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	    goto FailedDay;
    	    	    break;
    	    	}
    	    FailedStartHour:
    	    	printf("Enter starting hour: ");
    	    	(void)scanf("%d", &hourInput);
    	    	read_line(inputInfo, 4);
    	    	if (!(0 <= hourInput && hourInput <= 12)) {
    	    	    fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
    	    	    goto FailedStartHour;
    	    	}
    	    	switch (inputInfo[0])
    	    	{
    	    	case 'a':case 'A':
    	    	    info.ID[idInput].start = hourInput % 12;
    	    	    break;
    	    	case 'p':case 'P':
    	    	    info.ID[idInput].start = hourInput % 12 + 12;
    	    	    break;
    	    	default:
    	    	    fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
    	    	    goto FailedStartHour;
    	    	    break;
    	    	}
    	    FailedEndHour:
    	    	printf("Enter ending hour: ");
    	    	(void)scanf("%d", &hourInput);
    	    	read_line(inputInfo, 4);
    	    	if (!(0 <= hourInput && hourInput <= 12)) {
    	    	    fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
    	    	    goto FailedEndHour;
    	    	}
    	    	switch (inputInfo[0])
    	    	{
    	    	case 'a':case 'A':
    	    	    info.ID[idInput].end = hourInput % 12;
    	    	    break;
    	    	case 'p':case 'P':
    	    	    info.ID[idInput].end = hourInput % 12 + 12;
    	    	    break;
    	    	default:
    	    	    fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
    	    	    goto FailedEndHour;
    	    	    break;
    	    	}
    	    	info.ID[idInput].status = NotTaken;
    	    	break;
    	    }
    	    case NotTaken: {
    	    	char confirmation = 0;
    	    	do {
    	    	    printf("This Office Hour Already Exists\nWould You like to update? (Y\\N)");
    	    	    read_line(&confirmation, 1);
    	    	    if (confirmation == 'Y' || confirmation == 'y')
    	    	    	update_office_hour(tName, &idInput);
    	    	    else if (confirmation == 'n' || confirmation == 'N') {
    	    	    	printf("Exiting...");
    	    	    	break;
    	    	    }
    	    	    else
    	    	    	fprintf(stderr, "Please Answer Yes Or No\n");
    	    	} while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N');
    	    	break;
    	    }
    	    case Taken: {
    	    	char confirmation = 0;
    	    	do {
    	    	    printf("This Office Hour Already Exists & It is taken by %s \nWould You like to update? (Y\\N)", info.ID[idInput].takenby);
    	    	    read_line(&confirmation, 1);
    	    	    if (confirmation == 'Y' || confirmation == 'y') {
    	    	    	printf("Are you sure?");
    	    	    	read_line(&confirmation, 1);
    	    	    	if (confirmation == 'Y' || confirmation == 'y')
    	    	    	    update_office_hour((char *)&tName,&idInput);
    	    	    	else
    	    	    	    printf("Exiting...");
    	    	    }
    	    	    else if (confirmation == 'n' || confirmation == 'N') {
    	    	    	printf("Exiting...");
    	    	    	break;
    	    	    }
    	    	    else
    	    	    	fprintf(stderr, "Please Answer Yes Or No\n");
    	    	} while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N');
    	    	break;
    	    }
    	    default:
    	    	fprintf(stderr, "Unknown Error Occured while reading appointment data" __FILE__);
    	    	break;
    	    }
    	    store_office_hour();
    	}
    }
}
void update_office_hour(char* tName, int* id) {
    if (id != NULL) {
    	char inputInfo[10] = "";
    	int hourInput = -1;
    	printf("\tOffice hour ID: %d\n\tEnter 1 to update day, 2 to update start, 3 to update end: ", *id);
        _123:
    	read_line(inputInfo, 1);
    	switch (inputInfo[0])
    	{
    	case '1': {
    	    FailedDay1:
    	    printf("\t\tEnter new day: ");
    	    read_line(inputInfo, 10);
    	    switch (inputInfo[0])
    	    {
    	    case 'M':case 'm':
    	    	info.ID[*id].day = Monday;
    	    	break;
    	    case 't':case 'T':
    	    	if (inputInfo[1] == 'h' || inputInfo[1] == 'H')
    	    	    info.ID[*id].day = Thursday;
    	    	else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
    	    	    info.ID[*id].day = Tuesday;
    	    	else {
    	    	    fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	    goto FailedDay1;
    	    	}
    	    	break;
    	    case 'w':case 'W':
    	    	info.ID[*id].day = Wednesday;
    	    	break;
    	    case 'F':case 'f':
    	    	info.ID[*id].day = Friday;
    	    	break;
    	    case 'S':case 's':
    	    	if (inputInfo[1] == 'a' || inputInfo[1] == 'A')
    	    	    info.ID[*id].day = Saturday;
    	    	else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
    	    	    info.ID[*id].day = Sunday;
    	    	else {
    	    	    fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	    goto FailedDay1;
    	    	}
    	    	break;
    	    default:
    	    	fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
    	    	goto FailedDay1;
    	    	break;
    	    }
    	    printf("\t\tNew day is : ");
    	    switch (info.ID[*id].day)
    	    {
    	    case Monday:
    	    	printf("Monday");
    	    	break;
    	    case Tuesday:
    	    	printf("Tuesday");
    	    	break;
    	    case Wednesday:
    	    	printf("Wednesday");
    	    	break;
    	    case Thursday:
    	    	printf("Thursday");
    	    	break;
    	    case Friday:
    	    	printf("Friday");
    	    	break;
    	    case Saturday:
    	    	printf("Saturday");
    	    	break;
    	    case Sunday:
    	    	printf("Sunday");
    	    	break;
    	    default:
    	    	fprintf(stderr, "An Error Occured in Date Printing" __FILE__);
    	    	break;
    	    }
    	    break;
    	}
    	case '2': {
    	    FailedStartHour:
    	    printf("Enter starting hour: ");
    	    (void)scanf("%d", &hourInput);
    	    read_line(inputInfo, 4);
    	    if (!(0 <= hourInput && hourInput <= 12)) {
    	    	fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
    	    	goto FailedStartHour;
    	    }
    	    switch (inputInfo[0])
    	    {
    	    case 'a':case 'A':
    	    	info.ID[*id].start = hourInput % 12;
    	    	break;
    	    case 'p':case 'P':
    	    	info.ID[*id].start = hourInput % 12 + 12;
    	    	break;
    	    default:
    	    	fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
    	    	goto FailedStartHour;
    	    	break;
    	    }
    	    break;
    	}
    	case '3': {
    	    FailedEndHour:
    	    printf("Enter ending hour: ");
    	    (void)scanf("%d", &hourInput);
    	    read_line(inputInfo, 4);
    	    if (!(0 <= hourInput && hourInput <= 12)) {
    	    	fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
    	    	goto FailedEndHour;
    	    }
    	    switch (inputInfo[0])
    	    {
    	    case 'a':case 'A':
    	    	info.ID[*id].end = hourInput % 12;
    	    	break;
    	    case 'p':case 'P':
    	    	info.ID[*id].end = hourInput % 12 + 12;
    	    	break;
    	    default:
    	    	fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
    	    	goto FailedEndHour;
    	    	break;
    	    }
    	    break;
    	}
    default:
    	fprintf(stderr, "Update Operation Select Failed\nPlease select from the table below\n[1]-Update Day\n[2]-Update Starting Hour\n[3]-Update Ending Hour");
        goto _123;
	break;
    }
    }
    else {
        int id2 = -1;
        id2=find_office_hour(tName);
        if(0>id2)
        fprintf(stderr,"Find Office Hour Failed");
        else{
            char filename[name+4]="";
            strcpy(filename,tName);
            strcat(filename,".dat");
            if(NULL!=(file=fopen(filename,"rb+"))){
            update_office_hour((char *)&tName, &id2);
            store_office_hour();
            }else
            fprintf(stderr,"File open failed \"%s\" line:%s",filename,__LINE__);
        }
    }
}
void print_office_hour(char *tName,bool IsStudent) {
    char filename[name+4]="";
    strcpy(filename, tName);
    strcat(filename,".dat");
    if(NULL==(file=fopen(filename,"rb+")))
    	fprintf(stderr,"An Error Occured While Opening File \"%s\"\nFunciton:Print Office Hour %s",filename, __FILE__);
    else{
    	fread(&info,sizeof(info),1,file);
    	printf("\tID number\tDay\t\tStart\t\tEnd\n");
    	if (IsStudent == false) {
    	    {
    	    for (int i = 0; i < maxOffice; i++)
    	    	if (info.ID[i].status == DNE);
    	    	else {
    	    	    printf("\t%d\t\t", i);
    	    	    switch (info.ID[i].day)
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
    	    	    if (info.ID[i].start < 12)
    	    	    	printf("%d a.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	    else
    	    	    	printf("%d p.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	    printf("\t\t");
    	    	    if (info.ID[i].end < 12)
    	    	    	printf("%d a.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	    else
    	    	    	printf("%d p.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	    printf("\n");
    	    	}
    	    }
    	}else
    	for (int i = 0; i < maxOffice; i++)
    	{
    	    if (info.ID[i].status != NotTaken);
    	    else {
    	    	printf("\t%d\t\t", i);
    	    	switch (info.ID[i].day)
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
    	    	if (info.ID[i].start < 12)
    	    	    printf("%d a.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	else
    	    	    printf("%d p.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	printf("\t\t");
    	    	if (info.ID[i].end < 12)
    	    	    printf("%d a.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	else
    	    	    printf("%d p.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	printf("\n");
    	    }
    	}
    	fclose(file);
    }   
}