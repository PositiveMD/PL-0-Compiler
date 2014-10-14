#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_TOKEN_SIZE 12

/*
Constants must store kind, name, and value

Variables must store kind, name, L, and M

Procedures must store kind, name, L, and M
*/
typedef struct symbol
{
    int kind;       //const = 1 , var = 2, proc = 3
    char name[12];  //name up to 11 chars
    int val;        //number (ASCII value)
    int level;      //L Level
    int addr;       //M address
} symbol;

//The enums with all the token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} token_types;

typedef enum{

    CONSTANT = 1, VARIABLE, PROCEDURE
} symbol_table_type;

//The enums with all the opCode types
typedef enum {
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} op_code_types;

static char token[MAX_TOKEN_SIZE];
static symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
static int symbolTableCount;
static FILE *ifp;

void emit(int opCode, int level, int m, FILE *ofp, FILE *ofp2, int printPars);

void getToken();

void printError(int errorCode);

void addToSymbolTable(int type, char *identifier, int param);

void constDeclaration();

void term();

void evaluateExpression();

void factor();

void varDeclaration();

void evaluateCondition();

void executeBody();

void block(FILE *ofp, FILE *ofp2, int printPars);

void convertToMCode(FILE *ofp, FILE *ofp2, int printPars);








