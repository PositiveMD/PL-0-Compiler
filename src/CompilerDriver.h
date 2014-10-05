#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef COMPILERDRIVER

#define MAX_NUM_LENGTH 5
#define MAX_IDENT_LENGTH 11


#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct instruction{
    int op; // opcode
    int l;  // L
    int m;   // M
}instruction;

//The enums with all the token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} token_types;

static int stack[MAX_STACK_HEIGHT];
static int bar[MAX_STACK_HEIGHT];
static instruction code[MAX_CODE_LENGTH];

static instruction IR;
static int BP;
static int SP;
static int PC;
static int OLD_PC;
static int HALT;

//Fetches the next instruction from the code array
void fetch();

//Finds the base L levels down
int base(int l, int base); // l stands for L in the instruction format

//Finds the valid operation of the OPR based on the M value
void OPRFINDER();

//Returns the string version of the opcode
const char * opcodeFinder(int opcode);

//Outputs the current variables and  etc
void outputStack(FILE *ofp);

//Executes the P-Machine
void execute();

//Removes the comments from the input file and moves it the a commentless input file
void removeComments(FILE *ifp, FILE *ofp1, FILE *ofp4);

//Takes the clean input file and converts it into tokens
void convertToToken(FILE *ifp, FILE *ofp2, FILE *ofp3, FILE *ofp4);

//Starts the Scanner
void startScanner(int printLex);

//Starts the PMachine
void startPMachine(int printStack);


#define COMPILERDRIVER
#endif
