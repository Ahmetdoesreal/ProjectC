/*
readline.c

2300005633
Ahmet Deniz Sezgin
Line Reader

Reading lines was never more harder.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "readline.h"
void read_line(char* stringin,int maxnum) {
	char input = 0;
	int curnum = 0;
	do {
		input = getchar();
	} while (input == '\n' || input == ' ' || input == '\t');
	do{
		*(stringin+curnum) = input;
		curnum++;
		input = getchar();
	} while (input!='\n'&&curnum<maxnum-1);
	while (input != '\n') {
		input = getchar();
	}
	return;
	//for (int i = 0; i < maxnum; i++)
	//{
	//	printf("%c\t%d\t%d\n", stringin[i], stringin[i], i + 1);
	//}
}