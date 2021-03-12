//+
//	lab0user.c - ELEC377 Demo User Module
//
//	This program reads the results produced by our /proc module
//	and formats it for the user.
//
//	Author: Thomas R. Dean
//-

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define FILENAME "/proc/Lab0"

//+
// Funtion main
// 
// Purpose: This function reads the file /proc/Lab0
//   The contents of the file is the unix time stamp giving the
//   time when the system was booted. It uses the ctime library
//   to convert the string to a readable form and prints an
//   appropriate message.
//
// Parameters:
//	none
//
// Returns int
//-

int main()
{
    FILE * stream;
    int second;
    time_t sec;

    // open the file
    stream = fopen(FILENAME,"r");

    if (stream == NULL){
	fprintf(stderr,"Could not open %s\n",FILENAME);
	exit(-1);
    }
    // read the number of seconds from the /proc file
    fscanf(stream,"%d",&second);

    // cast it into a time_t

    sec = (time_t) second;

    // Convert to a string and print

    printf("The System was booted at %s\n",ctime(&sec));
    fclose(stream);
    
    return 0;
}
