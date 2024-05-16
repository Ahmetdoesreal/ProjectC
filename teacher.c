#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "teacher.h"
#include "readline.h"
#define name (64+1)
#define maxOffice 5
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
		scanf("%d", &idInput);
		if (!(0<=idInput&&idInput < maxOffice))
			fprintf(stderr, "Can't add more office hours!\nConsider Updating existing hours");
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
				scanf("%d", &hourInput);
				read_line(inputInfo, 4);
				if (!(0 <= hourInput && hourInput <= 12)) {
					fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
					goto FailedStartHour;
				}
				switch (inputInfo[0])
				{
				case 'a':case 'A':
					info.ID[idInput].start = hourInput % 12;
					//printf("set start hour to %d a.m [%d]", hourInput, info.ID[idInput].start);
					break;
				case 'p':case 'P':
					info.ID[idInput].start = hourInput % 12 + 12;
					//printf("set start hour to %d p.m [%d]", hourInput, info.ID[idInput].start);
				default:
					fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
					goto FailedStartHour;
					break;
				}
			FailedEndHour:
				printf("Enter ending hour: ");
				scanf("%d", &hourInput);
				read_line(inputInfo, 4);
				if (!(0 <= hourInput && hourInput <= 12)) {
					fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
					goto FailedEndHour;
				}
				switch (inputInfo[0])
				{
				case 'a':case 'A':
					info.ID[idInput].end = hourInput % 12;
					//printf("set end hour to %d a.m [%d]", hourInput, info.ID[idInput].end);
					break;
				case 'p':case 'P':
					info.ID[idInput].end = hourInput % 12 + 12;
					//printf("set end hour to %d a.m [%d]", hourInput, info.ID[idInput].end);
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
							update_office_hour(&tName,&idInput);
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
			scanf("%d", &hourInput);
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
			scanf("%d", &hourInput);
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
		char filename[name + 4] = "";
		strcpy(filename, tName);
		strcat(filename, ".dat");
		if (NULL == (file = fopen(filename, "rb+")))
			fprintf(stderr, "An Error Occured While Opening File \"%s\"\nFunciton:Print Office Hour %s", filename, __FILE__);
		else {
			fread(&info, sizeof(info), 1, file);
			int id2 = -1;
			printf("Enter identification number: ");
			scanf("%d", &id2);
			if (id2 > maxOffice || id2 < 0)
				fprintf("Entered ID:%d must be in legal range [%d-%d]", id2, 0, maxOffice);
			else if (info.ID[id2].status == DNE)
				fprintf("Entered ID:%d does not exist", id2);
			else if (id2 < maxOffice && id2 >= 0) {
				char inputInfo[10] = "";
				int hourInput = -1;
				printf("\tOffice hour ID: %d\n\tEnter 1 to update day, 2 to update start, 3 to update end: ", id2);
			_123_2:
				read_line(inputInfo, 1);
				switch (inputInfo[0])
				{
				case '1': {
				FailedDay:
					printf("\t\tEnter new day: ");
					read_line(inputInfo, 10);
					switch (inputInfo[0])
					{
					case 'M':case 'm':
						info.ID[id2].day = Monday;
						break;
					case 't':case 'T':
						if (inputInfo[1] == 'h' || inputInfo[1] == 'H')
							info.ID[id2].day = Thursday;
						else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
							info.ID[id2].day = Tuesday;
						else {
							fprintf(stderr, "Day Selection failed \n\a(Please refine your input)");
							goto FailedDay;
						}
						break;
					case 'w':case 'W':
						info.ID[id2].day = Wednesday;
						break;
					case 'F':case 'f':
						info.ID[id2].day = Friday;
						break;
					case 'S':case 's':
						if (inputInfo[1] == 'a' || inputInfo[1] == 'A')
							info.ID[id2].day = Saturday;
						else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
							info.ID[id2].day = Sunday;
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
					printf("\t\tNew day is : ");
					switch (info.ID[id2].day)
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
				FailedStartHour1:
					printf("Enter starting hour: ");
					scanf("%d", &hourInput);
					read_line(inputInfo, 4);
					if (!(0 <= hourInput && hourInput <= 12)) {
						fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
						goto FailedStartHour1;
					}
					switch (inputInfo[0])
					{
					case 'a':case 'A':
						info.ID[id2].start = hourInput % 12;
						break;
					case 'p':case 'P':
						info.ID[id2].start = hourInput % 12 + 12;
						break;
					default:
						fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)");
						goto FailedStartHour1;
						break;
					}
					break;
				}
				case '3': {
				FailedEndHour1:
					printf("Enter ending hour: ");
					scanf("%d", &hourInput);
					read_line(inputInfo, 4);
					if (!(0 <= hourInput && hourInput <= 12)) {
						fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
						goto FailedEndHour1;
					}
					switch (inputInfo[0])
					{
					case 'a':case 'A':
						info.ID[id2].end = hourInput % 12;
						break;
					case 'p':case 'P':
						info.ID[id2].end = hourInput % 12 + 12;
						break;
					default:
						fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)");
						goto FailedEndHour1;
						break;
					}
					break;
				}
				default:
					fprintf(stderr, "Update Operation Select Failed\nPlease select from the table below\n[1]-Update Day\n[2]-Update Starting Hour\n[3]-Update Ending Hour");
					goto _123_2;
					break;
				}
			}
			store_office_hour();
		}
	}
}
void print_office_hour(char *tName) {
	char filename[name+4]="";
	strcpy(filename, tName);
	strcat(filename,".dat");
	if(NULL==(file=fopen(filename,"rb+")))
		fprintf(stderr,"An Error Occured While Opening File \"%s\"\nFunciton:Print Office Hour %s",filename, __FILE__);
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
		fclose(file);
	}

}