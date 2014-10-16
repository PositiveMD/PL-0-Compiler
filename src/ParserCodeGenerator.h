#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_TOKEN_SIZE 12
#define MAX_CODE_LENGTH 500

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

typedef enum{

    CONSTANT = 1, VARIABLE, PROCEDURE
} symbol_table_type;

//The enums with all the opCode types
typedef enum {
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} op_code_types;

typedef enum {
    OPR_RET = 0, OPR_NEG, OPR_ADD, OPR_SUB, OPR_MUL, OPR_DIV, OPR_ODD, OPR_MOD,
    OPR_EQL, OPR_NEQ, OPR_LSS, OPR_LEQ, OPR_GTR, OPR_GEQ
} opr_type;

static char token[MAX_TOKEN_SIZE];
static symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
static int symbolTableCount;
static FILE *ifp, *ofp, *ofp2;
static int printPars;
static int codeCount;
static instruction code[MAX_CODE_LENGTH];

int symbolAddress(int symbolPosition);

int symbolLevel(int symbolPosition);

int symbolType(int symbolPosition);

void emit(int opCode, int level, int m);

void getToken();

int find(char *ident);

void printError(int errorCode);

void addToSymbolTable(int type, char *identifier, int param);

void constDeclaration();

void term();

void evaluateExpression();

void factor();

void varDeclaration();

void evaluateCondition();

void statement();

void block(FILE *ofp, FILE *ofp2);

void convertToMCode(FILE *ofp, FILE *ofp2);
