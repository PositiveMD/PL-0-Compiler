#ifndef PARSER_H_
#define PARSER_H_
#define MAX_SYMBOL_TABLE_SIZE 100
#define CODE_SIZE 100
#define CONST 1
#define INT 2
#define PROC 3

// Symbol Table
typedef struct{
	int kind; 	// const = 1, int = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val;	// number (ASCII VALUE)
	int level;	// L level
	int addr; 	// M address
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

// Functions
void Program();
void Block();
void Statement();
void Condition();
void Expression();
void Term();
void Factor();
void nextToken();
int checkIdentType();
void emit(int op, int l, int m);
void printSymbolTable();
void printGeneratedCode();
void printParserErrors(int errorNum);
symbol getSymbol();
void updateTable(symbol temp);
void removeVariables();

// Globals
int currTokenLocation = 0;	// Index of array of tokens
int cx = 0;			// Index of generated code array
int ctemp;			// Temp holders for index of generated code array
int numSymbols = 0;		// Number of symbols in symbol table
int m = 3;			// Holds address of incomming identifiers
int CIT;			// Determins if valid variable is used
symbol tempSymbol;		// Holds a temp symbol
symbol Osymbol;			// Holds the symbol that is being output
symbol Isymbol;			// Holds the symbol that is being input
int currLine = 1; 		// Current line
int lexilvl = 0;		// current lexigraphical level
int first = 1;			// 0 if lexilvl is greather than 0

// Intermediate Code variabl
typedef struct{
	int op;		//opcode
	int l;		// Lexicalgraphical level
	int m;		// modifier
} code;

char *instructions[] = {" ", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO"};
code codeStack[CODE_SIZE];
code IR;


#endif /* PARSER_H_*/
