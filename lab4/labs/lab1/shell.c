//+
// File:	shell.c
//
// Pupose: 	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//
//		if the command is not recognized an error is printed.
//-

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

int splitCommandLine(char * commandBuffer, char* args[], int maxargs);
void doCommand(char * args[], int nargs);

//+
// Function:    main
//
// Purpose: 	Starting point of the program. This contains
//		the command loop. Removes the newline, splits
//		the command line into words, and executes the command.
//		Exits on EOF, or if the exit function is called.
//-

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){
        //printf("%s",commandBuffer);

	// remove newline at end of buffer
	int cmdLen = strlen(commandBuffer);
	if (commandBuffer[cmdLen-1] == '\n'){
	    commandBuffer[cmdLen-1] = '\0';
            //printf("<%s>\n",commandBuffer);
	}

	// split command line into words.
	int nargs = splitCommandLine(commandBuffer, args, MAXARGS);

	// debugging
	//printf("%d\n", nargs);
	//int i;
	//for (i = 0; i < nargs; i++){
	//   printf("%d: %s\n",i,args[i]);
	//}

	if (nargs != 0){
	    doCommand(args, nargs);
	}

	// print prompt
	printf("%%> ");
	fflush(stdout);
    }
}

////////////////////////////// String Handling ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	This function skips over a given char in a string
//		For security, will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars
//		ID function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
       if (skip == '\0') return charPtr;
       while (*charPtr == skip){
          charPtr++;
       }
       return charPtr;
}

//+
// Funtion:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of argyments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char * commandBuffer, char* args[], int maxargs){
   int numArgs = 0;
   char * curPos = commandBuffer;
   do {
       // skip leading blanks
       curPos = skipChar(curPos, ' ');
       // check for an argument
       if (*curPos != '\0'){
          // *curpos not null means one more argument
	  if (numArgs == maxargs){
	     fprintf(stderr,"exceeded max number of arguments(%d)\n",maxargs);
	     return 0;
	  }
	  // save pointer to argument
	  args[numArgs++] = curPos;

	  // find next space, if it exists
	  // overwrite with null character to terminate argument string
	  char * nextSpace = strchr(curPos, ' ');
	  if (nextSpace == NULL){
	     // no space in remainder of string, so this is
	     // the last argument, move the curPos pointer
	     // to the end of the string;
	      curPos = strchr(curPos,'\0');
	  } else {
	      // move curPos to next space
	      curPos = nextSpace;
	      // ovewrite with null and move to next char.
	      *(curPos++) = '\0';
	  }
       }
   } while(*curPos != '\0');
   return numArgs;
}


////////////////////////////// Command Handling ///////////////////////////////////

// define command handling function pointer type
typedef void(*commandFunc)(char * args[], int nargs);

// associate a command name with a command handling function
typedef struct {
   char 	* cmdName;
   commandFunc 	cmdFunc;
} cmdStruct;

// prototypes for command handling functions
void exitFunc(char * args[], int nargs);
void pwdFunc(char * args[], int nargs);
void lsFunc(char * args[], int nargs);
void cdFunc(char * args[], int nargs);

// list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.
cmdStruct commands[] = {
   { "exit", exitFunc},
   { "pwd",  pwdFunc},
   { "ls",   lsFunc},
   { "cd",   cdFunc},
   { NULL, NULL}		// terminator
};

//+
// Function:	doCommand
//
// Purpose:	This command calls a command handling funciton from
//		the commands array based on the first argument passed
//		in the args array.
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

void doCommand(char * args[], int nargs){
    int i = 0;
    while(commands[i].cmdName != NULL){
       //printf("<%s>\n", commands[i].cmdName);
       //printf("<%s>\n", args[0]);
       if (strcmp(commands[i].cmdName,args[0]) == 0){
          (*commands[i].cmdFunc)(args,nargs);
	  return;
       }
       i++;
    }
    fprintf(stderr,"Command not recognized\n");
}

//////////////////////////////////////////////////
//            comand Handling Functions         //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameters and reutrn values.
//////////////////////////////////////////////////

//+
// Function:	all handling funcionts
//
// Purpose:	this command performs the funcdtion
//		associated with the commands.
//		The name of the comomand is in first argument.
//	 	Since it is called by doCommand, it is known
//		that arg is at least one element long
//
// Parameters:
//	args	command and parameters, an array of pointers to strings
//	nargs	number of entries in the args array
//
// Returns	nothing (void)
//-

//+
// Function:    exitFunc
// Purpose:	exit the shell
//-
void exitFunc(char * args[], int nargs){
    exit(0);
}

//+
// Function:    pwdFunc
// Purpose:     print the working directory
//-
void pwdFunc(char * args[], int nargs){
   char * cwd = getcwd(NULL,0); // getcwd allocates buffer
   printf("%s\n",cwd);
   free(cwd);
}

//+
// Function:    isHidden
// Purpose:     utility funcdtion for ls command Function
// 		returns true if the directory entry starts
// 		with a dot ('.')
// Prameters:	sturct dirent *d - pointer to directory entity structure
//
// Returns:	int (true or false)
//-

int isHidden(const struct dirent * d){
    if (d->d_name[0] == '.'){
       return 0;
    }
    return 1;
}

//+
// Function:    pwdFunc
// Purpose:     list the contents of the current directory
//  		default is to ignore hidden files
//  		argument "-a" means to list all files
//-

void lsFunc(char * args[], int nargs){
   int (*filter)(const struct dirent * d) = isHidden;
   if (nargs == 2) {
      if (strcmp(args[1], "-a") == 0){
          filter = NULL;
      }else {
          fprintf(stderr,"Unkonwn flag %s\n", args[1]);
	  return;
      }
   }
   if (nargs > 2){
          fprintf(stderr,"Too many arguments\n");
	  return;
   }
   struct dirent **namelist;
   int numEnt = scandir(".",&namelist,filter,NULL);
   int i;
   for(i = 0; i < numEnt; i++){
      printf("%s\n",namelist[i]->d_name);
   }
}

//+
// Function:    lsFunc
// Purpose:     change the current working directory
//   		default is to the home directory
//   		if an argument attempt to change
//   		to the given directory
//   		report an error if the directory does not exist
//-

void cdFunc(char * args[], int nargs){
    if (nargs == 1){
	struct passwd *pw = getpwuid(getuid());
	if (pw == NULL){
	    fprintf(stderr,"error retrieving user information\n");
	    return;
	}
	// check to make sure field exists
	if (strlen(pw->pw_dir) == 0){
	    fprintf(stderr,"home directory not set\n");
	    return;
	} else {
	    if (chdir(pw->pw_dir) != 0){
		fprintf(stderr,"could not change directory to %s\n",pw->pw_dir);
		return;
	    }
	}
    } else {
	if (chdir(args[1]) != 0){
	    fprintf(stderr,"could not change directory to %s\n",args[1]);
	    return;
	}
    }
}
