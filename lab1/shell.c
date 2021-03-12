//+
// File:	shell.c
//
// Purpose:	This program implements a simple shell program. It does not start
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

int main() {

    char commandBuffer[CMD_BUFFSIZE];
    char *args[MAXARGS];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while(fgets(commandBuffer,CMD_BUFFSIZE,stdin) != NULL){

	// Remove newline at end of buffer
	// TODO Step 2: remove newline from end of buffer
	int ind = 0;
	while(commandBuffer[ind]!='\n')ind++;
	commandBuffer[ind] = '\0';

	// Split command line into words.
	// TODO Step 2: call splitCommandLine with the right parameters
	int nargs = splitCommandLine(commandBuffer, args, MAXARGS);
	//printf("nargs = %d\n", nargs);
	
	// Debugging for step 2
	printf("%d\n", nargs);
	int i;
	for (i = 0; i < nargs; i++){
       	  printf("%d: %s\n",i,args[i]);
	}

	// Execute the command
	// Remember to check if there is a command (i.e. value of nargs)
	// TODO: Step 3 call doCommand with the right arguments

        doCommand(args, nargs);

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
//		For security, the function will not skip null chars.
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars.
//		Identity function if the string doesn't start with skip,
//		or skip is the null character
//-

char * skipChar(char * charPtr, char skip){
 // TODO Step 2: skip over instances of the char skip
 //    return input value of charPtr if *char is null char
 if(skip == '\0') return charPtr;

 while(*charPtr == skip && *charPtr != '\0')
 {
 	charPtr++;
 }

 return charPtr;
 
}

//+
// Function:	splitCommandLine
//
// Purpose:	This splits a string into an array of strings.
//		The array is passed as an argument. The string
//		is modified by replacing some of the space characters
//		with null characters to terminate each of the strings.
//
// Parameters:
//	commandBuffer	The string to split
//	args		An array of char pointers
//	maxargs		Size of array args (max number of arguments)
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char* commandBuffer, char* args[], int maxargs){
   // TODO Step 2 split the command into words using only
   // the functions skipChar and strchr. You may use fprintf to report
   // errors that are encountered to stderr.

   int ctr = 0;
   int i = 0;
   char* fSpace;
  
   while(*commandBuffer != '\0')
   {
      args[i] = skipChar(commandBuffer, ' ');
      if(*args[i] != '\0')ctr++;
      
      commandBuffer = args[i];

      fSpace = strchr(commandBuffer, ' ');
      if(fSpace == NULL)
      {
         return ctr;
      }

      commandBuffer = fSpace;
      *commandBuffer = '\0';
      commandBuffer++;
      
      i++;
   }

   return ctr;

}


////////////////////////////// Command Handling ///////////////////////////////////

// typedef for pointer to command handling functions

// TODO STEP 3a write the typedef for the function handler pointer to
// use in the structure immediately below.
// See the description of the function prototypes at the bottom of
// the file in the comments.

typedef void  (*fhp)(char* args[], int nargs);

// cmdStruct type:
// Associates a command name with a command handling function

// TODO STEP 3b use the typedef above (Step 3a) to make a two element
// struct. The first is a char * for the name of the command
// the other is a function pointer for the command handling function
// typedef struct {
//
// } typename;

typedef struct cmdStruct
{
   char* name;
   fhp funPoint;
}cmdStruct;


// prototypes for command handling functions
// TODO STEP 4b,6: add a function prototype
// for each command handling function

void exitFunc(char* args[], int nargs);
void pwd(char* args[], int nargs);
void ls(char* args[], int nargs);
void cd(char* args[], int nargs);
int filterFunc(const struct dirent *d);
// Array that provides the list commands and functions
// must be terminated by {NULL, NULL} 
// in a real shell, this would be a hashtable.

// TODO Step 4a: add a global array of 
// the type above that contains initializers
// of strings and command handling funciton names

cmdStruct comArray[] =
{
   {"ls", ls},
   {"cd", cd},
   {"pwd", pwd},
   {"exit", exitFunc},
   {NULL, NULL}
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
   // TODO Step 5 this function is small
   //  this is the command search loop
   int i = 0;
   int temp = 0;
   
   while(comArray[i].name != NULL)  
   {
   temp = strcmp(comArray[i].name, args[0]);
   //printf("temp = %d\n", temp);
   //printf("%s %s", comArray[i].name, args[0]);
      if(strcmp(comArray[i].name, args[0]) == 0)
      {
         (*comArray[i].funPoint)(args, nargs);
	 return;
      }
   i++;   
   }
   printf("Error: No match for %s\n", args[0]);
   return;
}

//////////////////////////////////////////////////
//            command Handling Functions        //
//////////////////////////////////////////////////
// all command handling functions have the same
// parameter types and return values.
//////////////////////////////////////////////////

//+
// Function:	all handling functions
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

// TODO step 4b put command handling function for exit here
// e.g. void exitFunc(char * args[], int nargs){
// }

void exitFunc(char* args[], int nargs)
{
   exit(0);
}

// TODO step 6 put rest of command handling functions here

void pwd(char* args[], int nargs)
{
   char* cwd = getcwd(NULL,0);
   printf("%s\n", cwd);
   free(cwd);
}

void ls(char* args[], int nargs)
{
   int i;
   if(nargs == 1)
   {
      int i = 0;
      struct dirent** namelist;
      int numEnts = scandir(".", &namelist, filterFunc, NULL);
      for(i = 0; i < numEnts; i++)printf("%s\n", namelist[i]->d_name);
   }
   else if(nargs >= 2)
   {
      if(strcmp(args[1],"-a") != 0)
      {
         printf("Invalid Second Argument\n");
	 return;
      }
      struct dirent **namelist;
      int numEnts = scandir(".", &namelist, NULL, NULL);
      for(i = 0; i < numEnts; i++)printf("%s\n", namelist[i]->d_name);
   }
}

int filterFunc(const struct dirent *d)
{
   char* dot = (*d).d_name;
   if(*dot == '.')return 0;
   return 1;
}


void cd(char* args[], int nargs)
{
   if(nargs == 1)
   {
      struct passwd *pw =  getpwuid(getuid());
      if(pw == NULL)
      {
         printf("No Home Directory\n"); 
      }
      int i = chdir(pw->pw_dir);
      if(i != 0)printf("Bleh\n");
   }         
   else if(nargs >= 2)
   {
      int i = chdir(args[1]);
      if(i != 0)
      {
         printf("Directory Does Not Exist\n");
      }	 
   }
}
