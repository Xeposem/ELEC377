//+
// Name:  consumer.c
//
// Purpose: this file implements the consumer using a
//   shared segement and a simple implementatin of mutual exclusion.
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
// Purpose: This is the main function. It implements the conumer using a
//   shared segement and a simple implementatin of mutual exclusion.
//
// Parameters:
//	argc - count of command line arguments (ignored)
//	argv - array of command line arguments (ignored)
//
// Parameters:
//      int
//-
int main (int argc, char *argv[]){

    // initialize the shared memory, load in the initial array's, spawn the worker
    // processes.

    key_t   key;
    struct shared *sharedPtr;
    int shmid;
    int numProd;
    int charRead;
    int c;

    /*	 Shared memory init 	*/
    key = ftok(".", 'S');
    if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
	if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
	    fprintf(stderr,"Error allocating shared memory. \n");
	    exit(1);
	}
    }

    // now map the region..
    if((int)(sharedPtr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	fprintf(stderr,"Couldn't map the memory into our process space.\n");
	exit(1);
    }
	
    return 0;
}

