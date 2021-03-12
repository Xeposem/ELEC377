//+
// Name: common.c
//
// Purpose: contains routines to be used by both the producer,
//       and the consumer. 
//       Mutual Exclusion routines are here.
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
// Function: test_and_set
//
//  Purpose: C wrapper for the xmpxchng machine languate instruction.
//   Reads the value of the lock variable and sets it to 1.
//
//  Paramters:
//	lock - the address of the shared lock variable
//
//  Returns
//	The value of lock variable from the exchange.
//-

int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}

//+
//  getMutex - The C implemenation of the spinlock test
//   and set approach. This function busy waits until the
//   lock is false and then sets it to true.
//
//  Paramters:
//	lock - the address of the shared lock variable
//
//  Returns
//	Nothing
//-

void getMutex(int * lock){
}

//+
//  releaseMutex - The C implemenation of the spinlock test
//   and set approach. This sets the lock to false.
//
//  Paramters:
//	lock - the address of the shared lock variable
//
//  Returns
//	Nothing
//-

void releaseMutex(int * lock){
    // pass the key to the next process
}
