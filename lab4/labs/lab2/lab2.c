//+
// Module:  lab2.c
//
// Purpose: This file provides a kernel module that 
//     prints the list of active processes giving
//     the process id, the userid, the total number
//     of VM pages and the number of VM pages that are resident.
//
//-

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

//+
// Name: my_read_proc:
//
//  Purpose:
//   this function that will be called by the kernel when
//   the user reads the /proc/lab2 file. The first time the proc
//   is read (fpos is 0) we return the headers and the data for
//   the first process. On subsequent calls we return the data
//   for the next process. We return EOF when we have reached
//   the first proc.
//
//   Parameters:
//       page - the buffer provided by the kernel
//	 start - point to pointer to indicate which buffer was  used
//	 fpos - the postion in the file
//	 blen - the length of page (ignored)
//	 eof  - pointer to indicate end of file
//	 data - module private data pointer (ignored)
//
//   Return value:
//	int - The number of bytes written into the buffer
//-

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){

    int kPerPage = (PAGE_SIZE >> 10);
    int numChars = 0;
    //printk("<> entering readproc fpos = %d\n",fpos);
    if (fpos == 0){
	// write headers
	numChars = sprintf(page, "PID\tUID\tVSZ\tRSS\n");

        // find first task
        firstTask = &init_task; 
	while(firstTask -> pid == 0){
	    firstTask = firstTask -> next_task;
	}
	theTask = firstTask;

	numChars += sprintf(page + numChars, "%4d\t%4d", theTask->pid, theTask -> uid);
	if (theTask->mm != NULL){
	    numChars += sprintf(page + numChars, "\t%4d\t%4d\n",
	    theTask->mm->total_vm * kPerPage,
	    theTask->mm->rss * kPerPage);
	} else {
	    numChars += sprintf(page + numChars, "\t   0\t   0\n");
	}

        // advance to next task
	do {
	   theTask = theTask -> next_task;
        } while (theTask -> pid == 0);

    } else {
        if (theTask == firstTask){
            *eof = 0;
            *start = page;
            return 0;
        }

	// write task info for one task
	numChars = sprintf(page , "%4d\t%4d", theTask->pid, theTask -> uid);
	if (theTask->mm != NULL){
	    numChars += sprintf(page + numChars, "\t%4d\t%4d\n",
	    theTask->mm->total_vm * kPerPage, theTask->mm->rss * kPerPage);
	} else {
	    numChars += sprintf(page + numChars, "\t   0\t   0\n");
	}

	 // advance to next task
	do {
	    theTask = theTask -> next_task;
        } while (theTask -> pid == 0);
    }
    *eof = 1;
    *start = page;
    //printk("<> returning %d from read proc\n",numChars);
    return numChars;
}

//+
// Name: init_module
//
//  Purpose:
//	Called by the kernel to initialize
//      any data structures used by the module when
//      the module is loaded. In this case we will
//      add our file entry to the /proc file system
//
//  Parameters:
//	None
//
// Return value
//      int - 0 on success, non-zero for failure
//-

int init_module(){
   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   if(proc_entry == NULL) {
      return -1;
   }
   proc_entry->read_proc = my_read_proc;
}

//+
// Name: cleanup_module
//
// Purpose:
//	 Called by the kernel to deallocate
//      any data structures used by the module when
//      the module is removed from the kernel. In our
//      case we remove the entry from the /proc file system
//
//  Parameters:
//	None
//
// Return value
//      none
//-

void cleanup_module(){ 
   remove_proc_entry("lab2", NULL);
}
