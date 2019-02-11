/*
filename 	: 	sortapp.c
@description	: 	application in user space to call "sort" system call (sys call number-399), 
			which uses quicksort algorithm (O(nlogn)) to perform sorting of array from 
			greatest to the smallest.
reference	:	for time: https://www.includehelp.com/c-programs/system-date-time-linux.aspx
*/


#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>

int main()
{
long ret;		/*Return value of the syscall*/
int16_t size;		/*Size of the array passed by the user*/
srand(time(0));		/*Seeding rand()*/
size=rand() & 0xfff;	/*feeding the size of the array from 0-4095*/
int32_t unsorted[size];	
int32_t sorted[size];

time_t get_T= time(NULL);
struct  tm tm = *localtime(&get_T);
     
printf("Curent Process Id:%d\n",syscall(20));
printf("Current User Id: %d\n",syscall(24));

printf("System Date is: %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
printf("System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);

printf("(MAIN)Contents of the array before calling sort function i.e. unsorted array\n");

for(int i=0;i<size;i++)
{
unsorted[i]=rand() & 0xfff;
printf("(MAIN)The value at %d position is %d\n",i,unsorted[i]);
}

printf("(MAIN)Checking for error conditions- (Passing 0 as size)\n");
ret=syscall(399,unsorted,sorted,0);
perror("Invalid parameter");

printf("(MAIN)Checking for error conditions- (Passing NULL as sorted array)\n");
ret=syscall(399,unsorted,NULL,0);
perror("Invalid parameter");

printf("(MAIN)Now impleneting proper sys call with proper parameters\n");
ret=syscall(399,unsorted,sorted,size); /*Calling the sort syscall*/
perror("Invalid parameter");

printf("(MAIN)Contents of the array after calling sort function i.e. sorted array\n");

for(int i=0;i<size;i++)
{
printf("The value at %d position is %d\n",i,sorted[i]);
}

return 0;
}
