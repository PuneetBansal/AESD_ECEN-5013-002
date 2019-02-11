/*
@filename 	:file1.c
@description 	:Perform file operations on linux platform and cross compile for beaglebone
@author		:Puneet Bansal
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 					/*Header file for using chmod*/

#define FILE_NAME "fileprac.txt"
#define MAX_SIZE 1000
#define BUFFER_SIZE 10

int main()
{
FILE* fptr1=NULL;
char *arr;
char ch;
char buffer[BUFFER_SIZE];

printf("Welcome to Command Line\n");
FILE* fptr= fopen(FILE_NAME,"w");			/*Create the file*/
if(fptr==NULL)
	{	
	printf("File creation unsuccessfull\n");
	}
else
	{
	printf("File creation successfull\n");
	}

chmod(FILE_NAME,0700);					/*Changing file permissions*/
fptr=fopen(FILE_NAME,"w");


printf("\nPutting character P in the file\n");
fputs("P",fptr); 					/*Putting the character in file */
fclose(fptr); 

fptr1=fopen(FILE_NAME,"a");				/*Opening the file in append mode*/
arr= malloc(MAX_SIZE);

printf("\nEnter the string you want to enter into the file\n");
fgets(arr, MAX_SIZE, stdin);				/*get the string input from user*/
fputs(arr,fptr1);					/*put the string entered by user in arr*/

fflush(fptr1);						/*Flush file output*/

fclose(fptr1);

fptr1=fopen(FILE_NAME,"r");				/*Open the file in read mode*/

ch=fgetc(fptr1);
printf("\nThe character read from file is %c\n",ch);

fgets(buffer,BUFFER_SIZE,fptr1);
printf("\nThe string read from the file is %s\n",buffer);
fclose(fptr1);
free(arr);

return 0;
}

