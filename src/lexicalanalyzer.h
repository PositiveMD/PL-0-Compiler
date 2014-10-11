#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

// Constants
#define CMAX 12         // Maximum number of character length + \0 symbol
#define DMAX 5          // Maximum number of digit length
#define LMAX 200        // Maximum line length
#define ERRMAX 4        // Maximum error number
#define TRUE 1
#define FALSE 0

// Token types
typedef enum    {nulsym = 1, identsym, numbersym, plussym, minussym,
                 multsym, slashsym, oddsym, eqlsym, neqsym, lessym, leqsym,
                 gtrsym, geqsym, lparentsym, rparentsym, commasym,
                 semicolonsym, periodsym, becomessym,  beginsym , endsym,
		ifsym, thensym, whilesym, dosym, callsym, constsym, intsym,
		procsym, outsym, insym, elsesym } token_type;

char * symbolesName[] = {"", "nulsym", "identsym", "numbersym", "plussym", "minussym",
			 "multsym", "slashsym", "oddsym", "eqlsym", "neqsym", "lessym",
			 "leqsym", "grtsym", "geqsym", "lparentsym", "rparentsym", "commasym",
			 "semicolonsym", "periodsym", "becomessym", "beginsym", "endsym",
			 "ifsym", "thensym", "whilesym", "dosym", "callsym", "constsym",
			 "intsym", "procsym", "outsym", "insym", "elsesym"};

typedef enum {VARERR = 0, NUMERR, VARLENERR, SYMBERR} error_type;

struct token{
        int kind;
        char name[CMAX+1];
	int value;
	int hasValue;
};

// Reserved words
char* reservedWords [] ={ "begin", "end", "if", "then", "while", "do", "call", "const",
			  "int", "proc", "out", "in", "else"};
int numReservWords = sizeof(reservedWords) / sizeof(char *);               // number of keywords


// Name of Special Symboles
char * sSymbolesName[] = {"plus", "minus", "times", "slash", "rparetn",
                          "lparent", "rbracket", "lbracket", "equal", "nequal", "comman","period", "lss", "grt", "leq", "geq", "semicolon", "colon"};
int numSpecials = sizeof(sSymbolesName)/ sizeof(char *);        // number of special characters

// Global Variables
char currChar;                  // Current character
char line[LMAX];                // Current line
int charCount;                  // Character counter
int lineCount;                  // line counter
int h;				// index of nameRec array
char previous;
int invalidVar = FALSE;

// Used in the parser
struct token currToken;

struct token nameRec[500];
int errors[ERRMAX];

FILE* ifp, *ofp;


// Functions
void errorFound(int type);                      // Mark an error
void initialize();                              // Initialize variables
void nextCharacter();                           // Description in source file.
void skip();                                    // Skips whitespace and comments
void getToken();				// Gets the token
void printLexemeTable();
void skipComments();
void printLexemeList();
int printErrors();
void printSymbolicList();


#endif /* LEXICALANALYZER_H_ */
