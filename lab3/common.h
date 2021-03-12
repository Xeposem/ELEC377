//+
//  common.h - Common definiton of the shared
//     memory segment and prototypes for semaphore functions
// 
//  Created by Thomas Dean
//  Copyright 2005 Queen's University.
//-

#define MEMSIZE 200
#define BUFFSIZE 5

struct shared {

//Needed for synchronization
int lock;

//Keep track of how many producers are generating data
int numProducers;

//Shared buffer
char buffer[BUFFSIZE];
int in;
int out;
int count;

};

//+
//
//  Forces a process to wait until the shared resources is available and there is mutual exclusion
//
//-
void getMutex(int * lock);

//+
//
//  Sets the mutex back to its initial state so another process may claim it and begin.
//
//-
void releaseMutex(int * lock);
