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

typedef struct symbol
{
    int kind;       //const = 1 , var = 2, proc = 3
    char name[12];  //name up to 11 chars
    int val;        //number (ASCII value)
    int level;      //L Level
    int addr;       //M address
} symbol;


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

int main(int argc, char *argv[])
{

	int i;
	int printPars = 0;

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






	return 0;
}