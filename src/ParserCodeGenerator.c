/*
Anthony Dinh
Alvin Lam
COP 3402 : System Software
*/

#include "ParserCodeGenerator.h"

//Prints the machine code to the file
void emit(int opCode, int level, int m, FILE *ofp, FILE *ofp2, int printPars)
{

    fprintf(ofp, "%d %d %d\n", opCode, level, m);

    if (printPars)
        fprintf(ofp2, "%d %d %d\n", opCode, level, m);

}

void getToken()
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
            printf("call must be followed by an identifier.");
            exit(1);
        case 15:
            printf("Call of a constant or variable is meaningless.");
            exit(1);
        case 16:
            printf("then expected.");
            exit(1);
        case 17:
            printf("Semicolon or } expected.");
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
        case 26:
            printf("Declaration must end with ;");
            exit(1);
        case 27:
            printf("begin must be closed with end");
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
        //symbolTable[symbolTableCount].addr = param;
        symbolTable[symbolTableCount].level = 0;
    }

    symbolTableCount++;



}


//Declares a constant
void constDeclaration()
{
    char *temp;
    char *constName;

    int value;

    do{

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        strcpy(constName, token);

        getToken();

        if (atoi(token) != eqsym)
            printError(3);

        getToken();

        if (atoi(token) != numbersym)
            printError(2);

        getToken();

        value = atoi(token);

        addToSymbolTable(CONSTANT, constName, value);

        getToken();

    }while(atoi(token) == commasym);

    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();
}

void term()
{

    char *temp;

    factor();

    while ((atoi(token) == multsym) || (atoi(token) == slashsym)){
        getToken();
        factor();
    }

}

void evaluateExpression()
{
    char *temp;

    if ((atoi(token) == plussym) || (atoi(token) == minussym)){

        getToken();
        term();
    }

    while ((atoi(token) == plussym) || (atoi(token) == minussym)){

        getToken();
        term();
    }
}

void factor()
{
    char *temp;

    if (atoi(token) == identsym)
        getToken();

    else if (atoi(token) == numbersym)
        getToken();

    else if (atoi(token) == lparentsym){

        getToken();
        evaluateExpression();

        if (atoi(token) != rparentsym)
            printError(22);

        getToken();
    }

    else
        printError(23);
}

void varDeclaration()
{

    char *temp;
    char *varName;

    do{

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        strcpy(varName, token);

        addToSymbolTable(VARIABLE, varName, 0);

        getToken();


    }while(atoi(token) == commasym);

    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();

}

void evaluateCondition()
{
    char *temp;

    if (atoi(token) == oddsym){

        getToken();
        evaluateExpression();

    }

    else{

        evaluateExpression();

        if ((atoi(token) != eqsym) || (atoi(token) != neqsym ) ||
            (atoi(token) != lessym) || (atoi(token) != leqsym) ||
            (atoi(token) != gtrsym) || atoi(token) != geqsym)
            printError(20);

        getToken();

        evaluateExpression();



    }

}

void executeBody()
{
    char *temp;

    if (atoi(token) == identsym){

        getToken();

        if (atoi(token) != becomessym)
            printError(13);

        getToken();

        evaluateExpression();
    }

    //else if (TOKEN = call)

    else if (atoi(token) == beginsym){

        getToken();

        executeBody();

        while (atoi(token) == semicolonsym){

            getToken();
            executeBody();
        }

        if (atoi(token) != endsym)
            printError(27);

        getToken();


    }

    else if (atoi(token) == ifsym){

        getToken();

        evaluateCondition();

        if (atoi(token) != thensym)
            printError(16);

        getToken();

        executeBody();
    }

    else if (atoi(token) == whilesym){

        getToken();

        evaluateCondition();

        if (atoi(token) != dosym )
            printError(18);

        getToken();

        executeBody();
    }


}

void block(FILE *ofp, FILE *ofp2, int printPars)
{
    char *temp;

    if (atoi(token) == constsym)
        constDeclaration();

    if (atoi(token) == varsym)
        varDeclaration();

    //If Token = procedure

    executeBody();



}


void convertToMCode(FILE *ofp, FILE *ofp2, int printPars)
{

    char *temp;

    getToken();


    block(ofp, ofp2, printPars);

    if (atoi(token) != periodsym)
        printError(9);

}



int main(int argc, char *argv[])
{

	int i;
	int printPars = 0;

    symbolTableCount = 0;

    FILE *ofp, *ofp2;



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


    convertToMCode(ofp,ofp2,printPars);

    printf("No errors, program is syntactially correct.\n");

    fclose(ifp);
    fclose(ofp);
    fclose(ofp2);

	return 0;
}
