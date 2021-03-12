//+
//  Name: common.h
//
//  Purpose: Common definiton of the shared memory segment
//     and prototypes for semaphore functions
// 
//  Created by Thomas Dean
//  Copyright 2005 Queen's University.
//
//-

#define NUMPROCS 5
#define MEMSIZE 200
#define BUFFSIZE 5

//+
// Name: shared
//
// Purpose: 
//   Defines the variables in the shared memory segment
//
// Fields:
//    lock - the shared lock varible
//    buffer - the array used for the fifo queue
//    inPtr - the array index at which elements are inserted.
//    outPtr - the array index at which elements are removed.
//    count - the number of elements in the queue
//    prodCount - the number of running producers.
//-

struct shared {
};


void getMutex(int * lock);
void releaseMutex(int * lock);
