/*
Anthony Dinh
Alvin Lam
COP 3402 : System Software
*/

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


//Prints the machine code to the file
void emit(int opCode, int level, int m, FILE *ofp, FILE *ofp2, int printPars)
{

    fprintf(ofp, "%d %d %d\n", opCode, level, m);

    if (printPars)
        fprintf(ofp2, "%d %d %d\n", opCode, level, m);

}

void getToken(FILE *ifp)
{
    if (feof(ifp)){

        printf("We have reached the end of the file\n");
        exit(1);
    }

    fscanf(ifp, "%s", &token);

}

void printError(int errorCode)
{

    printf("Error %d: ", errorCode);

    //Prints the error based on the error code, then exits the program
    switch(errorCode){

        case 1:
            printf("Use = instead of :=.");
            exit(1);
        case 2:
            printf("= must be followed by a number.");
            exit(1);
        case 3:
            printf("Identifier must be followed by =.");
            exit(1);
        case 4:
            printf("const, var, procedure must be followed by identifier.");
            exit(1);
        case 5:
            printf("Semicolon or comma missing.");
            exit(1);
        case 6:
            printf("Incorrect sumbol after procedure declaration.");
            exit(1);
        case 7:
            printf("Statement expected.");
            exit(1);
        case 8:
            printf("Incorrect symbol after statement part in block.");
            exit(1);
        case 9:
            printf("Period expected.");
            exit(1);
        case 10:
            printf("Semicolon between statements missing.");
            exit(1);
        case 11:
            printf("Undeclared identifier.");
            exit(1);
        case 12:
            printf("Assignment to constant or procedure is not allowed.");
            exit(1);
        case 13:
            printf("Assignment operator expected.");
            exit(1);
        case 14:
            printf("call mus be followed by an identifier.");
            exit(1);
        case 15:
            printf("Call of a constant or variable is meaningless.");
            exit(1);
        case 16:
            printf("then expected.");
            exit(1);
        case 17:
            printf("Semicolon or } expetected.");
            exit(1);
        case 18:
            printf("do expected.");
            exit(1);
        case 19:
            printf("Incorrect symbol following statement.");
            exit(1);
        case 20:
            printf("Relational operator expected.");
            exit(1);
        case 21:
            printf("Expression must not contain a procedure identifier.");
            exit(1);
        case 22:
            printf("Right parenthesis missing.");
            exit(1);
        case 23:
            printf("The preceding factor cannot begin with this symbol.");
            exit(1);
        case 24:
            printf("The expression cannot begin with this symbol.");
            exit(1);
        case 25:
            printf("This number is too large.");
            exit(1);
        default:
            printf("Something broke in the error printer: %d ", errorCode);
            exit(1);

    }
}


//Adds a new value to the symbol table
void addToSymbolTable(int type, char *identifier, int param){

    symbolTable[symbolTableCount].kind = type;
    strcpy(symbolTable[symbolTableCount].name, identifier);

    if (type == 1)
        symbolTable[symbolTableCount].val = param;

    else {
        symbolTable[symbolTableCount].addr = param;
        symbolTable[symbolTableCount].level = 0;
    }



}


//Declares a constant
void constDeclaration(FILE *ifp)
{
    char *temp;
    char *constName;

    int value;

    do{

        getToken(ifp);

        if (strcmp(token, itoa(identsym, temp, 10)) != 0)
            printError(4);

        getToken(ifp);

        strcpy(constName, token);

        getToken(ifp);

        if (strcmp(token, itoa(eqsym, temp, 10)) != 0)
            printError(3);

        getToken(ifp);

        if (strcmp(token, itoa(numbersym, temp, 10)) != 0)
            printError(2);

        getToken(ifp);

        value = atoi(token);

        addToSymbolTable(CONSTANT, constName, value);

        getToken(ifp);
        
    }while(strcmp(token, itoa(commasym, temp, 10)) == 0);





}

void varDeclaration(FILE *ifp)
{
    
}

void block(FILE *ifp, FILE *ofp, FILE *ofp2, int printPars)
{
    char *temp;

    if (strcmp(token, itoa(constsym, temp, 10)) == 0)
        constDeclaration(ifp);

    if (strcmp(token, itoa(varsym, temp, 10)) == 0)
        varDeclaration(ifp);

    //If Token = procedure

}


void convertToMCode(FILE *ifp, FILE *ofp, FILE *ofp2, int printPars)
{

    char *temp;

    getToken(ifp);
    

    block(ifp, ofp, ofp2, printPars);

    if (strcmp(token, itoa(periodsym, temp, 10)) != 0)
        printError(9);

}



int main(int argc, char *argv[])
{

	int i;
	int printPars = 0;

    symbolTableCount = 0;

    FILE *ifp, *ofp, *ofp2;

    

	 //Checks to see if the number of arguments is correct
    if (argc > 2){

        printf("Invalid number of arguments: %d", argc);

        exit(1);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-a") == 0){

            printPars = 1;
            continue;

        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );
            exit(1);


        }

    }

    //Checks to see if the file exists
    if (!(access( "lexemelist.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(1);
    }


    ifp = fopen("lexemelist.txt", "r");
    ofp = fopen("mcode.txt", "w");
    ofp2 = fopen("output.txt", "a");


    convertToMCode(ifp,ofp,ofp2,printPars);

    fclose(ifp);
    fclose(ofp);
    fclose(ofp2);

	return 0;
}