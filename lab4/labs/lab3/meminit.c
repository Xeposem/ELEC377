//+
// Name:  meminit.c
//
// Purpose: This file creates and initializes a shared memory segment.
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1


//+
// Name:  main
//
// Purpose: This is the main function. It creates a shared memory
//   segment if it does not alreay exist. The id of the shared
//   memory segement is attached to the current directory. After
//   creration, it is mapped and initialized to Zero.
//
// Parameters:
//	argc - count of command line arguments (ignored)
//	argv - array of command line arguments (ignored)
//
// Parameters:
//      int
//-
int main (int c, char **v){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *memptr;
	FILE *inFile;
	int x,i, length, shmid, numReturned;


	/*	 Shared memory init 	*/
	errno = 0;
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		printf("The shared memory already exists.\n");
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(memptr = (struct shared *) shmat(shmid, 0, 0)) == -1){
		printf("Couldn't map the memory into our process space.\n");
		perror("foo");
        exit(1);
	}

	for (i = 0; i < MEMSIZE; i++){
	    ((char*)memptr)[i] = 0;
	}

	return 0;
}


