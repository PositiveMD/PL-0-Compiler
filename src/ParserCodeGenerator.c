/*
Anthony Dinh
Alvin Lam
COP 3402 : System Software
*/

#include "ParserCodeGenerator.h"



int symbolAddress(int symbolPosition)
{
    return symbolTable[symbolPosition].addr;
}

int symbolLevel(int symbolPosition)
{
    return symbolTable[symbolPosition].level;
}

int symbolType(int symbolPosition)
{
    return symbolTable[symbolPosition].kind;
}


/*
    Returns the postion of the symbol in symbol table.
    Returns 0 if the symbol is not found
*/
int find(char *ident)
{

    int i;

    for (i = symbolTableCount; i>=1; i--){

        if (strcmp(ident, symbolTable[i].name) == 0)
            return i;
    }

    return 0;
}

//Prints the machine code to the file
void emit(int opCode, int level, int m)
{
    if (codeCount > MAX_CODE_LENGTH)
        printError(25);

    code[codeCount].op = opCode;
    code[codeCount].l = level;
    code[codeCount].m = m;

    codeCount++;

}

//Prints the code table to a text file as input for the P-Machine
void printCodeTable()
{
    int i;

    for (i = 1; i < codeCount; i++){

        fprintf(ofp, "%d %d %d\n", code[i].op , code[i].l , code[i].m);
        fprintf(ofp2, "%d %d %d\n", code[i].op , code[i].l , code[i].m);

        if (printPars)
            printf("%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
}

void getToken()
{
    if (feof(ifp)){

        printf("We have reached the end of the file\n");
        exit(1);
    }

    fscanf(ifp, "%s", token);

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
        case 28:
            printf("This identifier has already been declared in the symbol table");
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
        symbolTable[symbolTableCount].addr = symbolTableCount;
        symbolTable[symbolTableCount].level = 1;
    }

    symbolTableCount++;
}

void term()
{
    int mulop;

    factor();

    while ((atoi(token) == multsym) || (atoi(token) == slashsym)){
        mulop = atoi(token);
        getToken();
        factor();

        if (mulop == multsym)
            emit(OPR, 0, OPR_MUL);
        else
            emit(OPR, 0, OPR_DIV);
    }

}

void factor()
{
    int symbolPosition;

    if (atoi(token) == identsym){

        getToken();
        symbolPosition = find(token);

        if (symbolPosition == 0)
            printError(11);

        if (symbolType(symbolPosition) == VARIABLE)
            emit(LOD, symbolLevel(symbolPosition) -1 , symbolAddress(symbolPosition));

        else if (symbolType(symbolPosition) == CONSTANT)
            emit(LIT, symbolLevel(symbolPosition), symbolAddress(symbolPosition));

        else {
            printError(24);
        }

        getToken();

    }

    else if (atoi(token) == numbersym){
        getToken();
        emit(LIT, 0, atoi(token));
        getToken();

    }


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


void evaluateCondition()
{

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

void evaluateExpression()
{
    int addop;

    if ((atoi(token) == plussym) || (atoi(token) == minussym)){

        addop = atoi(token);

        getToken();
        term();

        if (addop == minussym)
            emit(OPR, 0, OPR_NEG);
    }

    else{
        term();
    }

    while ((atoi(token) == plussym) || (atoi(token) == minussym)){

        addop = atoi(token);
        getToken();
        term();

        if (addop == plussym)
            emit(OPR, 0, OPR_ADD);
        else{
            emit(OPR, 0, OPR_SUB);
        }
    }
}

void statement()
{
    int symbolPosition;

    int cx1, cx2;

    int ctemp;

    if (atoi(token) == identsym){

        //Gets the identifier name
        getToken();

        symbolPosition = find(token);

        if (symbolPosition == 0)
            printError(11);

        if (symbolType(symbolPosition) != VARIABLE)
            printError(12);


        getToken();

        if (atoi(token) != becomessym){

            if (atoi(token) == eqsym)
                printError(1);

            printError(13);
        }


        getToken();

        evaluateExpression();

        emit(STO, symbolLevel(symbolPosition) -1, symbolAddress(symbolPosition));
    }

    //else if (TOKEN = call)

    else if (atoi(token) == beginsym){

        getToken();
        statement();


        while (atoi(token) == semicolonsym || atoi(token) == beginsym ||
            atoi(token) == ifsym || atoi(token) == whilesym ||
            atoi(token) == writesym || atoi(token) == readsym){

            if (atoi(token) == semicolonsym)
            getToken();

            statement();

        }

       // printf("%s\n", token );

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

        ctemp = codeCount;

        emit(JPC, 0, 0);

        statement();

        code[ctemp].m = codeCount;
    }

    else if (atoi(token) == whilesym){

        cx1 = codeCount;
        getToken();
        evaluateCondition();
        cx2 = codeCount;
        emit(JPC, 0, 0);

        if (atoi(token) != dosym )
            printError(18);

        getToken();

        statement();

        emit(JMP, 0, cx1);
        code[cx2].m = codeCount;

    }

    //Reads inputs from user and stores in identifier
    else if (atoi(token) == readsym){

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        symbolPosition = find(token);

        if (symbolPosition == 0)
            printError(11);

        if (symbolType(symbolPosition) != VARIABLE)
            printError(12);

        //SIO: reads input from user and stores at the top of the stack
        emit(SIO2, 0, 2);

        //Stores the value
        emit(STO, symbolLevel(symbolPosition) -1, symbolAddress(symbolPosition));

        getToken();


    }

    else if (atoi(token) == writesym){

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        symbolPosition = find(token);

        if (symbolPosition == 0)
            printError(11);

        if (symbolType(symbolPosition) == VARIABLE)
            emit(LOD, symbolLevel(symbolPosition) -1 , symbolAddress(symbolPosition));
        
        else
            emit(LIT, 0, symbolAddress(symbolPosition));


        emit(SIO, 0, 1);

        getToken();

    }


}

void varDeclaration()
{


    char varName[12];

    do{

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        strcpy(varName, token);

        if (!find(varName))
            addToSymbolTable(VARIABLE, varName, 0);

        else
            printError(28);

        getToken();



    }while(atoi(token) == commasym);



    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();

}

//Declares a constant
void constDeclaration()
{

    char constName[12];

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

        if (!find(constName))
            addToSymbolTable(CONSTANT, constName, value);

        else{

            printError(28);
        }


        getToken();

    }while(atoi(token) == commasym);

    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();
}

void block(FILE *ofp, FILE *ofp2)
{

    if (atoi(token) == constsym)
        constDeclaration();

    if (atoi(token) == varsym)
        varDeclaration();

    //If Token = procedure

    statement();
}


void convertToMCode(FILE *ofp, FILE *ofp2)
{

    getToken();

    block(ofp, ofp2);

    if (atoi(token) != periodsym)
        printError(9);

    printf("No errors, program is syntactially correct.\n");

}



int main(int argc, char *argv[])
{

	int i;
	printPars = 0;

    symbolTableCount = 1;

    codeCount = 1;

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
        printf("lexemelist.txt does not exist in the current directory\n");
        exit(1);
    }


    ifp = fopen("lexemelist.txt", "r");
    ofp = fopen("mcode.txt", "w");
    ofp2 = fopen("output.txt", "a");


    convertToMCode(ofp,ofp2);

    printCodeTable();

    fclose(ifp);
    fclose(ofp);
    fclose(ofp2);

	return 0;
}
