/*
readline.c

2300005633
Ahmet Deniz Sezgin
Line Reader

Reading lines was never more harder.
*/
#define _CRT_SECURE_NO_WARNINGS //makes it so Microsoft C Runtime Compiler doesnt complain about input functions
#include <stdio.h>              //standard c input output library
#include "readline.h"           //header file that contains the function prototypes

void read_line(char* stringin,int maxnum) {//the readline function itself
    // Variable to store current lenght and the character read by program
    char input = 0;
    int curnum = 0;
    // ignores the previous newlines and horizontal tabs and spaces
    // if the input is a anything useful (characters,numbers,signs)
    
    do {
    	input = getchar();
    } while (input == '\n' || input == ' ' || input == '\t');
    // since it already gets the value (assume its X)
    // it gets saved to the string or char called by other functions
    do{
    	*(stringin+curnum) = input; 
        //use of pointers allows for both string and a single character usage
        // If its a single character it is just a pointer
        // If its a string its a pointer acting as a array name


    	curnum++;//increase cursor
    	input = getchar();//gets next input
    } while (input!='\n'&&curnum<maxnum-1); 
    //if the value is either enter key or the previously decided size exceeded by the current cursor terminate the loop

    while (input != '\n')
    	input = getchar();
    //  ignores the characters if the given size exceded checks for enter previously to not prevent double enter presses
    //  example:(i type press  enter)
    //  if it was do{}while();
    //  it would run another getchar function
    return;
    //return without any values it is a void funciton no return value is expected
}