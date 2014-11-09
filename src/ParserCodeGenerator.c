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

    for (i = 0; i < codeCount; i++){

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
            printf("Use = instead of :=.\n");
            exit(1);
        case 2:
            printf("= must be followed by a number.\n");
            exit(1);
        case 3:
            printf("Identifier must be followed by =.\n");
            exit(1);
        case 4:
            printf("const, var, procedure must be followed by identifier.\n");
            exit(1);
        case 5:
            printf("Semicolon or comma missing.\n");
            exit(1);
        case 6:
            printf("Incorrect symbol after procedure declaration.\n");
            exit(1);
        case 7:
            printf("Statement expected.\n");
            exit(1);
        case 8:
            printf("Incorrect symbol after statement part in block.\n");
            exit(1);
        case 9:
            printf("Period expected.\n");
            printf("%s\n",token );
            exit(1);
        case 10:
            printf("Semicolon between statements missing.\n");
            exit(1);
        case 11:
            printf("Undeclared identifier.\n");
            exit(1);
        case 12:
            printf("Assignment to constant or procedure is not allowed.\n");
            exit(1);
        case 13:
            printf("Assignment operator expected.\n");
            exit(1);
        case 14:
            printf("call must be followed by an identifier.\n");
            exit(1);
        case 15:
            printf("Call of a constant or variable is meaningless.\n");
            exit(1);
        case 16:
            printf("then expected.\n");
            exit(1);
        case 17:
            printf("Semicolon or } expected.\n");
            exit(1);
        case 18:
            printf("do expected.\n");
            exit(1);
        case 19:
            printf("Incorrect symbol following statement.\n");
            exit(1);
        case 20:
            printf("Relational operator expected.\n");
            exit(1);
        case 21:
            printf("Expression must not contain a procedure identifier.\n");
            exit(1);
        case 22:
            printf("Right parenthesis missing.\n");
            exit(1);
        case 23:
            printf("The preceding factor cannot begin with this symbol.\n");
            exit(1);
        case 24:
            printf("The expression cannot begin with this symbol.\n");
            exit(1);
        case 25:
            printf("This number is too large.\n");
            exit(1);
        case 26:
            printf("Declaration must end with ;\n");
            exit(1);
        case 27:
            printf("begin must be closed with end\n");
            printf("%s\n",token);
            exit(1);

        default:
            printf("Something broke in the error printer: %d \n", errorCode);
            exit(1);

    }
}

//Removes symbols no longer in scope
void scopeCleanup()
{
    int count = 0;
    int tempSymbolTableCount = symbolTableCount;

    while (symbolTable[tempSymbolTableCount].level == currLevel){

        count++;
        tempSymbolTableCount--;
    }

    symbolTableCount -= count;



}


//Adds a new value to the symbol table
void addToSymbolTable(int type, char *identifier, int param){

    symbolTable[symbolTableCount].kind = type;
    strcpy(symbolTable[symbolTableCount].name, identifier);


    if (type == CONSTANT)
        symbolTable[symbolTableCount].val = param;

    else {
        symbolTable[symbolTableCount].addr = param;
        symbolTable[symbolTableCount].level = currLevel;
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
            emit(LOD, currLevel - symbolLevel(symbolPosition), symbolAddress(symbolPosition));

        else if (symbolType(symbolPosition) == CONSTANT)
            emit(LIT, 0, symbolTable[symbolPosition].val);

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

    int relOP;

    if (atoi(token) == oddsym){

        getToken();
        evaluateExpression();

        emit(OPR, 0, OPR_ODD);

    }

    else{

        evaluateExpression();

        switch (relOP = atoi(token)){

            case eqsym:
                getToken();
                break;
            case neqsym:
                getToken();
                break;
            case lessym:
                getToken();
                break;
            case leqsym:
                getToken();
                break;
            case gtrsym:
                getToken();
                break;
            case geqsym:
                getToken();
                break;
            default:
                printError(20);

        }        

        evaluateExpression();

        switch (relOP){

            case eqsym:
                emit(OPR, 0, OPR_EQL);
                break;
            case neqsym:
                emit(OPR, 0, OPR_NEQ);
                break;
            case lessym:
                emit(OPR, 0, OPR_LSS);
                break;
            case leqsym:
                emit(OPR, 0, OPR_LEQ);
                break;
            case gtrsym:
                emit(OPR, 0, OPR_GTR);
                break;
            case geqsym:
                emit(OPR, 0, OPR_GEQ);
                break;
            default:
                printf("Something went wrong with the relational emitter");

        }
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

        emit(STO, currLevel - symbolLevel(symbolPosition), symbolAddress(symbolPosition));
    }

    else if (atoi(token) == callsym){

        getToken();

        //We are expecting an identifier
        if (atoi(token) != identsym)
            printError(14);

        getToken();

        symbolPosition = find(token);

        //0 indicates that the identifier does not exist in the symbol table
        if (atoi(token) == 0)
            printError(11);

        //You're only allowed to call procedures
        if (symbolType(symbolPosition) != PROCEDURE)
            printError(15);

        emit(CAL, currLevel - symbolLevel(symbolPosition), symbolAddress(symbolPosition));

        getToken();

    }

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

      //printf("%s\n", token );

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

        if (atoi(token) == elsesym){

            ctemp = codeCount;
            emit(JMP, 0 ,0);
            statement();

            code[ctemp].m = codeCount;

        }

        
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
        emit(STO, currLevel - symbolLevel(symbolPosition), symbolAddress(symbolPosition));

        getToken();


    }

    else if (atoi(token) == writesym){

        getToken();

        evaluateExpression();

        emit(SIO, 0, 1);

        //getToken();

    }


}

void procDeclaration()
{
    char procName[12];

    while(atoi(token) == procsym){

        getToken();

        //Expected an identifier symbol after procedure declaration
        if (atoi(token) != identsym)
            printError(4);

        getToken();

        strcpy(procName, token);

        addToSymbolTable(PROCEDURE, procName, codeCount + 1);

        getToken();

        if (atoi(token) != semicolonsym)
            printError(5);

        getToken();

        block();

        if (atoi(token) != semicolonsym)
            printError(5);

        getToken();
    }

    scopeCleanup();


}

int varDeclaration()
{
	int varCount = 0;

    char varName[12];

    do{

        getToken();

        if (atoi(token) != identsym)
            printError(4);

        getToken();

        strcpy(varName, token);

        varCount++;
        addToSymbolTable(VARIABLE, varName, varCount + STO_CONSTANT);

        getToken();



    }while(atoi(token) == commasym);



    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();

    return varCount;

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

        addToSymbolTable(CONSTANT, constName, value);

        getToken();

    }while(atoi(token) == commasym);

    if (atoi(token) != semicolonsym)
        printError(26);

    getToken();
}

void block()
{
	currLevel++;

	int spaceToAllocate = 4;
	int jmpaddr = codeCount;
	emit(JMP,0 ,0);
	
    if (atoi(token) == constsym)
        constDeclaration();

    if (atoi(token) == varsym)
        spaceToAllocate += varDeclaration();

    if (atoi(token) == procsym)
    	procDeclaration();

    code[jmpaddr].m = codeCount;

    emit(INC, 0, spaceToAllocate);

    statement();

    emit(OPR, 0, OPR_RET);

    currLevel--;
}


void convertToMCode()
{

    getToken();

    block();

    if (atoi(token) != periodsym)
        printError(9);

    //emit(SIO3, 0, 3);

    printf("No errors, program is syntactially correct.\n");
    fprintf(ofp2, "No errors, program is syntactially correct.\n\n");

}



int main(int argc, char *argv[])
{

	int i;
	printPars = 0;

    symbolTableCount = 1;

    codeCount = 0;
    currLevel = -1;

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


    convertToMCode();

    printCodeTable();

    fclose(ifp);
    fclose(ofp);
    fclose(ofp2);

	return 0;
}
