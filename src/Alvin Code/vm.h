#ifndef VM_H_
#define VM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants;
#define MAX_STRING_LENGTH 30
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
//TODO figure out how to implement MAX_LEXI_LEVELS? this isn't present in the rest of the code...
//Does this mean that at most you can search 3 levels deep, or that the program should only be allowed to run 3 levels deep?
//Professor says that this means that there should be at most MAX_LEXI_LEVELS static linkages deep for a given call.
//There is currently no bookkeeping done for how deep a stack goes, so this may not end up getting implemented machine side, or at all.

#define ERROR_INVALID_OPCODE 1
#define ERROR_INVALID_STACK_OPERATION 2
#define ERROR_INVALID_FILENAME 3
#define ERROR_PC_TOO_HIGH 4
#define ERROR_PROG_TOO_LONG 5
#define ERROR_SP_OUT_OF_BOUNDS 6
#define ERROR_INVALID_IO_OPERATION 7

//String arrays that match the opcodes and operation values in index values
const char* OPCODE_STRINGS[] = {"FCH", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO"};
const char* STACK_OPERATION_STRINGS[] = {"RET", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
const char* SIO_OPERATION_STRINGS[] = {"", "SOT", "SIN"};

// Funcations;
int base (int l, int base);
void printCodeList();
void printCodeLine(int line);
void printStack();
void executeNextInstruction();
void operateOnStack();
void standardIOOperation();
void loadProgramFromFile(char filename[]);
void runProgram();

// Enumerations
enum OPCODE {FCH, LIT, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO};
enum STACK_OPERATION {RET, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ};
enum SIO_OPERATION {SOT = 1, SIN}; //only 1 and 2 have been defined in the specification for the machine


// Global Arrays;
int stack[MAX_STACK_HEIGHT];

// Registers
int BP = 1;
int SP = 0;
int PC = 0;

// other global variables
int code_size = 0;

#endif
