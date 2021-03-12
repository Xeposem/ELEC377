/*+:
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){

    int numChars;
    if (fpos == 0){
        //write headers
	numChars = sprintf(page, "     PID");
	numChars += sprintf(page + numChars, "     UID");
	numChars += sprintf(page + numChars, "     VSZ");
	numChars += sprintf(page + numChars, "     RSS\n");
	//find first valid task
        theTask = &init_task;
	while(theTask->pid == 0)theTask = (theTask->next_task);
	firstTask = theTask;
	//write task
        numChars += sprintf(page + numChars, "%8d", theTask->pid);
	numChars += sprintf(page + numChars, "%8d", theTask->uid);
	//check mm
	if(theTask->mm == NULL)
           numChars += sprintf(page + numChars, "       0       0\n");
	else
	{
	   int pSize = PAGE_SIZE >> 10;
	   int vm = pSize*(theTask->mm->total_vm);
	   int rss = pSize*(theTask->mm->rss);
           numChars += sprintf(page + numChars, "%8d", vm);
	   numChars += sprintf(page + numChars, "%8d\n", rss);
	}
	//point to next valid task
	do{
           theTask = theTask->next_task;
//	   if(theTask == firstTask) return* eof;
	}while(theTask->pid == 0);
    } else {
        if (theTask == firstTask){
            *eof = 0;
            *start = page;
            return 0;
        }
	  // write task info for one task
	  numChars = sprintf(page, "%8d", theTask->pid);
	  numChars += sprintf(page + numChars, "%8d", theTask->uid);
	  if(theTask->mm == NULL)
	     numChars += sprintf(page + numChars, "       0       0\n");
	  else
	  {
	     int pSize = PAGE_SIZE >> 10;
	     int vm = pSize*(theTask->mm->total_vm);
	     int rss = pSize*(theTask->mm->rss);
	     numChars += sprintf(page + numChars, "%8d", vm);
	     numChars += sprintf(page + numChars, "%8d\n", rss);
	  }
	  // advance to next task
	  do{
	     theTask = theTask->next_task;
	  }while(theTask->pid == 0);   
    }
    *eof = 1;
    *start = page;
    return numChars;
}

int init_module(){
   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   if(proc_entry == NULL){
      return -1;
   }
   proc_entry -> read_proc = my_read_proc;
   return 0;
}

void cleanup_module(){
   remove_proc_entry("lab2", NULL);
}
