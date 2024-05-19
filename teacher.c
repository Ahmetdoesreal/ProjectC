/*
appointment.c

2300005633
Ahmet Deniz Sezgin
Office Hour Appointment System

The program tracks the appointment of office hours

*/
#define _CRT_SECURE_NO_WARNINGS //makes it so Microsoft C Runtime Compiler doesnt complain about input functions
#include <stdio.h>              //Standard C Input Output Library
#include <string.h>             //Standard string library
#include <stdbool.h>            //Standard Boolean library
#include "teacher.h"            //Includes Header file that contains function prototypes for teacher functions
#include "readline.h"           //Includes Header file that contains function prototype for readline

//This Project is multiple files using #define makes it easier to change values
#define name (32+1)     //Defines Max Name Length 
#define maxOffice 20    //Defines Maximum Office Hour per teacher
#define FIND_FILE_FAIL (-20)
#define FIND_NOT_FOUND (-10)
#define FIND_RANGE_FAIL (-30)
struct TeacherInfo{ //          structure to store information
    char Name[name];//          teacher name
    struct OfficeHourInfo{//    structure to store office hour information
    	int start, end;//       declaration start and end hour variables
    	enum { Monday=1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }day;//enumeration starts at 1 and continues with 2 (i fill data with zeroes so the change is intentional)
        //enumeration to store values for day & status (it is much easier this way[For Me])
    	enum{DNE,Taken,NotTaken}status;//(0 is intentionally set to DNE)
    	char takenby[name];//store student name that claimed the office hour 
    }ID[maxOffice];//structure stored as array
};
struct TeacherInfo info;//global variable to avoid declaring it many times
FILE *file;

//find_office_hour helper function returns integer(id or reasons of exit)
int find_office_hour(char* tName){//gets teacher name as parameter
    
    //create another string a bit longer than the teacher name add .dat at the end
    char filename[name+4]="";
    strcpy(filename,tName);
    strcat(filename,".dat");

    FILE *find;
    struct TeacherInfo findInfo;
    if(NULL==(find=fopen(filename,"rb+"))){//if file open fails
    fprintf(stderr,"File Open failed \"%s\"\n",filename);//show error message to user
    return FIND_FILE_FAIL;//return fail value 
    }else{//if file open succesfull
        fread(&findInfo,sizeof(struct TeacherInfo),1,find);//reads teacher info
        fclose(find);//closes file
        int id=-1;//initializes id variable
        printf("\tEnter identification number: ");//prompts user for id
        (void)scanf("%d",&id);//gets info (void) to keep warning list clear
        if (id > maxOffice || id < 0){//check range
            fprintf(stderr,"Entered ID:%d must be in legal range [%d-%d]\n", id, 0, maxOffice-1);//shows error to user
            return FIND_RANGE_FAIL;//return fail value
        }
        else if(DNE!=findInfo.ID[id].status)//checks status
            return id;//if exists return id
        else{   //if not
                //shows error message
            fprintf(stderr,"Office hour does not exist\n");
            return FIND_NOT_FOUND;//return fail value
        }
    }
}

//store_office_hour helper function
void store_office_hour(void){
    rewind(file);//rewinds file to 0
    fwrite(&info, sizeof(info), 1, file);//writes data from structure to file
    fclose(file);//closes file
}
//insert office hour function
void insert_office_hour(char* tName) {//gets teacher name as parameter
    //initializes values
    int idInput = -1,hourInput=-1;
    char inputInfo[2]="" ;
    //initializes values

    //creates filename string adds .dat at the end
    char filename[name + 4] = "";
    strcpy(filename, tName);
    strcat(filename, ".dat");
    

    if (NULL == (file = fopen(filename, "rb+"))) {//checks if the file could be opened
    	fprintf(stderr, "An Error Occured While Opening File \"%s\"\nFunciton:Insert Office Hour %s",filename, __LINE__);//if fails gives out info and error message
    }
    else {//if succeedes
    	fread(&info, sizeof(info), 1, file);//reads teacher info
    	printf("\tEnter identification number: ");//prompts user
    	(void)scanf("%d", &idInput);//gets info
    	if (!(0<=idInput&&idInput < maxOffice))//checks if the id is in range
    	    fprintf(stderr, "Illegal Office Hour ID must be in range[%d-%d]",0,maxOffice-1);//prints out error message
    	else//if in range
    	{
    	    switch (info.ID[idInput].status)//checks status of inserted id
    	    {
    	    case DNE:{//if it does not exist
    	    FailedDay://if user somehow fail day entering go back to this point
    	    	printf("\tEnter a day: ");//prompts user
    	    	read_line(inputInfo, 3);//gets input
    	    	switch (inputInfo[0])//checks input
    	    	{
    	    	case 'M':case 'm':
    	    	    info.ID[idInput].day = Monday;
    	    	    break;
    	    	case 't':case 'T'://there is 2 days start with letter T
    	    	    if (inputInfo[1] == 'h' || inputInfo[1] == 'H')//check if its h or H to set it to Thursday
    	    	        info.ID[idInput].day = Thursday;
    	    	    else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')//or check if its u or U to set it to Tuesday
    	    	        info.ID[idInput].day = Tuesday;
    	    	    else {//if not both print out error message and go to the point
    	    	        fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
    	    	        goto FailedDay;
    	    	    }
    	    	    break;
    	    	case 'w':case 'W':
    	    	    info.ID[idInput].day = Wednesday;
    	    	    break;
    	    	case 'F':case 'f':
    	    	    info.ID[idInput].day = Friday;
    	    	    break;
    	    	case 'S':case 's'://there is 2 days start with letter S
    	    	    if (inputInfo[1] == 'a' || inputInfo[1] == 'A')//choose Saturday
    	    	    	info.ID[idInput].day = Saturday;
    	    	    else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')//Choose Sunday
    	    	    	info.ID[idInput].day = Sunday;
    	    	    else {//go to fail point and show message
    	    	    	fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
    	    	    	goto FailedDay;
    	    	    }
    	    	    break;
    	    	default://go to fail point and show message
    	    	    fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
    	    	    goto FailedDay;
    	    	    break;
    	    	}
    	    FailedStartHour://starting hour fail point
    	    	printf("\tEnter starting hour: ");//prompt user
    	    	(void)scanf("%d", &hourInput);//get input
    	    	read_line(inputInfo, 1);//get input again do decide am or pm
    	    	if (!(0 < hourInput && hourInput <= 12)) {//if the input of hour is in range of 12hr time (0-12] 0 is not included
    	    	    fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)\n");//print error message
    	    	    goto FailedStartHour;//go to fail point
    	    	}
    	    	switch (inputInfo[0])//check input data
    	    	{
    	    	case 'a':case 'A'://if a is first letter of the secondary input choose AM
    	    	    info.ID[idInput].start = hourInput % 12;//just remainder divided by hourinput
    	    	    break;
    	    	case 'p':case 'P'://choose PM
    	    	    info.ID[idInput].start = hourInput % 12 + 12;//remainder+12
    	    	    break;
                    //USE OF REMAINDER IS IMPORTANT SINCE 12AM IS 00:00 FOR 24 HR FORMAT
                    //                                AND 12PM IS 12:00 FOR 24 HR FORMAT 
    	    	default://if no match
    	    	    fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)\n");//show error message
    	    	    goto FailedStartHour;//go to defined point
    	    	    break;
    	    	}
    	    FailedEndHour://another point
    	    	printf("\tEnter ending hour: ");//go to that point
    	    	(void)scanf("%d", &hourInput);//get input
    	    	read_line(inputInfo, 1);//input 2
    	    	if (!(0 < hourInput && hourInput <= 12)) {//check hour range
    	    	    fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)\n");
    	    	    goto FailedEndHour;
    	    	}
    	    	switch (inputInfo[0])//check am pm
    	    	{
    	    	case 'a':case 'A':
    	    	    info.ID[idInput].end = hourInput % 12;//if am convert to 24hr accordingly
    	    	    break;
    	    	case 'p':case 'P':
    	    	    info.ID[idInput].end = hourInput % 12 + 12;//if pm convert to 24hr accordingly
    	    	    break;
    	    	default:
    	    	    fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)\n");//error message
    	    	    goto FailedEndHour;
    	    	    break;
    	    	}
    	    	info.ID[idInput].status = NotTaken;//set the status to not taken
    	    	break;
    	    }
    	    case NotTaken: {//not taken means it exists and avalible
    	    	char confirmation = 0;
                printf("This Office Hour Already Exists\nWould You like to update? (Y\\N): ");//asks confirmation
    	    	do {
    	    	    read_line(&confirmation, 1);//gets input
    	    	    if (confirmation == 'Y' || confirmation == 'y')
    	    	    	update_office_hour((char*)&tName, &idInput);//if yes go to update with teacher name and input id as a adress
    	    	    else if (confirmation == 'n' || confirmation == 'N') {
    	    	    	printf("Exiting...");//if no exits
    	    	    	break;
    	    	    }
    	    	    else
    	    	    	fprintf(stderr, "Please Answer Yes Or No\n");//if user cand decide yes or no
    	    	} while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N');//asks untill user decides
    	    	break;
    	    }
    	    case Taken: {//if the hour is claimed by a student
    	    	char confirmation = 0;
                printf("This Office Hour Already Exists & It is taken by %s \nWould You like to update? (Y\\N)", info.ID[idInput].takenby);
                //prompts user if the id is taken before by a student and gives the student name (maybe teacher decides to inform the student via an external mail or something)
    	    	do {
    	    	    read_line(&confirmation, 1);//gets input
    	    	    if (confirmation == 'Y' || confirmation == 'y') {//if yes
    	    	    	printf("Are you sure?");//asks again :D lets not make things harder by changing an existing appointment
    	    	    	read_line(&confirmation, 1);//gets input again and overrides previous
    	    	    	if (confirmation == 'Y' || confirmation == 'y')//if yes 
    	    	    	    update_office_hour((char *)&tName,&idInput);//calls update function
    	    	    	else//assumes the answer is no, again makes it easier for both ends byt not changing any information
    	    	    	    printf("Exiting...");
    	    	    }
    	    	    else if (confirmation == 'n' || confirmation == 'N') {//if the answer is no exits
    	    	    	printf("Exiting...");
    	    	    	break;
    	    	    }
    	    	    else
    	    	    	fprintf(stderr, "Please Answer Yes Or No\n");//in case user fails to answer a yes no question
    	    	} while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N');//loop conditions
    	    	break;
    	    }
    	    default:
    	    	fprintf(stderr, "Unknown Error Occured while reading appointment data %s:%d",__FILE__,__LINE__);//if the status doesnt match prints out error
    	    	break;
    	    }
    	    store_office_hour();//saves inserted/updated office hour
    	}
    }
}
//update hour function works with teacher name and id pointer parameters
void update_office_hour(char* tName, int* id) {
    if (id != NULL) {//if the pointer points to anywhere other than NULL
        //initalizes values
    	char inputInfo[2] = "";
    	int hourInput = -1;
        if(info.ID[*id].status==Taken){//checks if it is taken
            printf("The office hour is taken by %s\nAre you sure to update?",info.ID[*id].takenby);//informs user
            do{
                read_line(inputInfo,1);//gets input
                switch(inputInfo[0])//checks input
                {
                case 'y':case'Y'://if input is yes
                break;//do nothing
                case 'n':case 'N'://if decides to not update 
                printf("Exiting...");//exits
                return;
                default:
                fprintf(stderr,"Please Choose (Y/N) : ");//tells user to choose
                break;
                }
            }while(!(inputInfo[0]=='n'&&inputInfo[0]=='N'&&inputInfo[0]=='y'&&inputInfo[0]=='Y'));
        }
        printf("\tOffice hour ID: %d\n\tEnter 1 to update day, 2 to update start, 3 to update end: ", *id);//gives info about selected id & prompts user
	_123://couldnt find name so named it _123 1 for day 2 for start 3 for end, underscore is just to make syntax work
	read_line(inputInfo, 1);//get input
	switch (inputInfo[0])//check input
	{
	case '1': {//if 1
		FailedDay://a point
		printf("\t\tEnter new day: ");//prompts user
		read_line(inputInfo, 3);//gets input
		switch (inputInfo[0])//checks input
		{
		case 'M':case 'm':
			info.ID[*id].day = Monday;
			break;
		case 't':case 'T'://tuesday or thursday
			if (inputInfo[1] == 'h' || inputInfo[1] == 'H')
				info.ID[*id].day = Thursday;
			else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
				info.ID[*id].day = Tuesday;
			else {
				fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
				goto FailedDay;
			}
			break;
		case 'w':case 'W':
			info.ID[*id].day = Wednesday;
			break;
		case 'F':case 'f':
			info.ID[*id].day = Friday;
			break;
		case 'S':case 's'://saturday or sunday
			if (inputInfo[1] == 'a' || inputInfo[1] == 'A')
				info.ID[*id].day = Saturday;
			else if (inputInfo[1] == 'u' || inputInfo[1] == 'U')
				info.ID[*id].day = Sunday;
			else {
				fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
				goto FailedDay;
			}
			break;
		default:
			fprintf(stderr, "Day Selection failed \n\a(Please refine your input)\n");
			goto FailedDay;
			break;
		}
		printf("\t\tNew day is : ");//informs user for new day
		switch (info.ID[*id].day)//decides what to print
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
	case '2': {//if selected start hour(2)
			FailedStartHour:
			printf("\t\tEnter starting hour: ");//prompt user
			(void)scanf("%d", &hourInput);//get input
			read_line(inputInfo, 1);//get input part 2
			if (!(0 < hourInput && hourInput <= 12)) {//check if the value is not legal
				fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)\n");
				goto FailedStartHour;
			}
			switch (inputInfo[0])//checks part 2 of input to decide am or pm
			{
			case 'a':case 'A':
					   //am calculations
				info.ID[*id].start = hourInput % 12;
				break;
			case 'p':case 'P':
					   //pm calculations
				info.ID[*id].start = hourInput % 12 + 12;
				break;
			default:
				fprintf(stderr, "Starting Hour Selection failed \n\a(Please refine your input)\n");
				goto FailedStartHour;
				break;
			}
			break;
		}
	case '3': {//if selected 3(ending hour update)
		FailedEndHour:
		printf("\t\tEnter ending hour: ");//prompt user
		   (void)scanf("%d", &hourInput);//gets input
		read_line(inputInfo, 1);//gets input p2
		if (!(0 < hourInput && hourInput <= 12)) {//check if input is not legal 
			fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)\n");
			goto FailedEndHour;
		}
		switch (inputInfo[0])//chooses am or pm depending on input p2
		{                    //does the correct calucations depending on am or pm to 24hr format
		case 'a':case 'A':
			info.ID[*id].end = hourInput % 12;
			break;
		case 'p':case 'P':
			info.ID[*id].end = hourInput % 12 + 12;
			break;
		default:
			fprintf(stderr, "Ending Hour Selection failed \n\a(Please refine your input)\n");
			goto FailedEndHour;
			break;
		}
		break;
		}
	   default:
		fprintf(stderr, "Update Operation Select Failed\nPlease select from the table below\n[1]-Update Day\n[2]-Update Starting Hour\n[3]-Update Ending Hour");//tells user to choose correctly
		goto _123;
	   break;
	   }
    }
    else {//if the pointer is NULL
    int id2 = -1;//initializes id2 variable
    id2=find_office_hour(tName);//calls find_office hour function decide if id2 exists or file open results
    if(0>id2)//only failure results in negative results
    fprintf(stderr,"Find Office Hour Failed");
    else{//if succeeded
        char filename[name+4]="";
        strcpy(filename,tName);
        strcat(filename,".dat");
        //tries to open the file once more but in write read mode
        if(NULL!=(file=fopen(filename,"rb+"))){
        update_office_hour((char *)&tName, &id2);//calls the update hour function again with a proper pointer
        store_office_hour();//stores the updated hour
        }else//if file couldn't open
        fprintf(stderr,"File open failed \"%s\" line:%s",filename,__LINE__);//prints out error
        }
    }
}
void print_office_hour(char *tName,bool IsStudent) {//gets teacher name and checks if the function called by a student or a teacher
    char filename[name+4]="";
    strcpy(filename, tName);
    strcat(filename,".dat");
    //data filename operations
    if(NULL==(file=fopen(filename,"rb")))//attempts opening file
    	fprintf(stderr,"An Error Occured While Opening File \"%s\"\nFunciton:Print Office Hour %s",filename, __FILE__);//prints error message
    else{
    	fread(&info,sizeof(info),1,file);//reads info from the data file
    	printf("\tID number\tDay\t\tStart\t\tEnd\n");//table titles
    	if (IsStudent == false) {   //if the calling function is a teacher
    	    {
    	    for (int i = 0; i < maxOffice; i++)//prints out info as a loop
    	    	if (info.ID[i].status == DNE);//if the hour doesnt exist doesnt print anything
    	    	else {//but if it exist(taken or nottaken)
    	    	    printf("\t%d\t\t", i);//id print
    	    	    switch (info.ID[i].day)//decide what to print as day
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
    	    	    printf("\t");   //      tab
    	    	    if (info.ID[i].start < 12)// decides how to print out hour if lower than 12
    	    	    	printf("%d a.m.", ((info.ID[i].start + 11) % 12) + 1);//does the calculations to convert 24hr to 12hr
    	    	    else
    	    	    	printf("%d p.m.", ((info.ID[i].start + 11) % 12) + 1);//does the calculations to convert 24hr to 12hr
    	    	    printf("\t\t");//tabbing
    	    	    if (info.ID[i].end < 12)// decides how to print out hour if lower than 12 [END HOUR]
    	    	    	printf("%d a.m.", ((info.ID[i].end + 11) % 12) + 1);//does the calculations to convert 24hr to 12hr
    	    	    else
    	    	    	printf("%d p.m.", ((info.ID[i].end + 11) % 12) + 1);//does the calculations to convert 24hr to 12hr
    	    	    printf("\n");//newline
    	    	}
    	    }
    	}else//if caller is student
    	for (int i = 0; i < maxOffice; i++)
    	{
    	    if (info.ID[i].status != NotTaken);//if the status is not taken print out info
    	    else {
    	    	printf("\t%d\t\t", i);//print id
    	    	switch (info.ID[i].day)//print day
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
    	    	if (info.ID[i].start < 12)//print hour start
    	    	    printf("%d a.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	else
    	    	    printf("%d p.m.", ((info.ID[i].start + 11) % 12) + 1);
    	    	printf("\t\t");
    	    	if (info.ID[i].end < 12)//print hour end
    	    	    printf("%d a.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	else
    	    	    printf("%d p.m.", ((info.ID[i].end + 11) % 12) + 1);
    	    	printf("\n");
    	    }
    	}
    	fclose(file);//close file
    }   
}