//+ 	ELEC 377, Lab 3
//
//   common.c contains routines to be used from both the
//   producer, and the  consumer
//   Mutual Exclusion routines will be here
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <asm/system.h>


#include "common.h"

#define FALSE 0
#define TRUE 1

//+
//
//  test_and_set writes 1 to a memory location and returns its old value as a single atomic operation.
//  This function prevents any other process from beginning until the first process's test-and-set is finished.
//
//-
int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}

//+
//
//  getMutex forces a process to wait until the shared resource is available and there is
//  mutual exclusion.
//
//-
void getMutex(int *  lock){
        while(test_and_set(lock));
}

//+
//
//  releaseMutex sets the mutex back to a initial state so another process may begin.
//
//-
void releaseMutex(int * lock){
	*lock = FALSE;
}

