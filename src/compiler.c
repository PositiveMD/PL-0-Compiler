/*
 * Alvin Lam
 * Anthony Dinh
 * COP3402 System Software
 */

#include "lexicalanalyzer.h"
#include "parser.h"
#include "vm.h"

int main(){

        char filename[30] = "input.txt";
//        printf("PL/O Compiler\n");
//        printf("Enter filename that contains source code: ");

//        scanf("%s", filename);

        ifp = fopen(filename, "r");

	// If input file exists
        if(ifp){

		// Create tokens
             	initialize();
		printf("\nSource Code:\n");
		while(!feof(ifp)){
			getToken();
		}

		printLexemeTable(); //31 40 41
		printLexemeList();

		// If errors are found the parser will not run
		if(printErrors() == TRUE)
			printf("\nParser will not execute because errors where found in source code\n");
		// Execution of Parser
		else{
			Program();
			printSymbolTable();
			printGeneratedCode();

			// Initializing VM
			stack[1] = 0;
			stack[2] = 0;
			stack[3] = 0;
			code_size = cx;

			// Print generated code
			printCodeList();
			printf("\n");
			//codeStack[25].m = 5;
			// Run Virtual machine
			runProgram();

			// Close input file
			fclose(ifp);

		}
		//printSymbolTable();

	}

	// File name was invalid
	else{
		printf("\ninvalid name\n");
	}

	return 0;
}

//VIRTUAL MACHINE BEGINS HERE
void printStack() {
	int numBasePointers = 1;
	int basePointers[700];
	int currentBP = BP;
	int i;

	while(currentBP > 1) {
		basePointers[700 - numBasePointers] = currentBP;
		currentBP = stack[currentBP+1];
		numBasePointers++;
	}

	numBasePointers--;

	for(i = 1; i <= SP; i++) {
		if (basePointers[700-numBasePointers]==i) {
			printf("| ");
			numBasePointers--;
		}
		printf("%d ", stack[i]);
	}
}

void printCodeList() {
	int i;
	printf("Virtual Machine Instructions\n");
	printf("  Line   OP     L     M\n");
	for(i = 0; i < code_size; i++){
		printCodeLine(i);
		printf("\n");
	}
}

void printCodeLine(int line) {
	printf("%5d %5s %5d %5d", line, OPCODE_STRINGS[codeStack[line].op], codeStack[line].l, codeStack[line].m);
}

void runProgram() {
	// Prints initial values
	printf("\t\t\t    PC   BP     SP   Stack\n");
	printf("Initial values  \t%5d %5d %5d ", PC, BP, SP);
	printStack();
	printf("\n");

	while(BP > 0) {	// If BP is 0, then the program has executed an OPR.RET from the base program, time to halt.
		if (PC < code_size) {
			printCodeLine(PC);
			executeNextInstruction();
			printf("\t%5d %5d %5d    ", PC, BP, SP);	// print registers
			printStack();
			printf("\n");
		} else if (PC >= MAX_CODE_LENGTH){
			exit(ERROR_PC_TOO_HIGH);
		} else {
			exit(ERROR_PC_TOO_HIGH);
		}
	}
}

int base(int l, int base) {	//find base L levels down
	int b1;
	b1 = base;
	while(l > 0){
		b1 = stack[b1];
		l--;
	}
	return b1;
}

void executeNextInstruction() {	// fetch code at PC and execute
	int inputtemp;
	symbol temp;
	// Fecth Cycle
	IR = codeStack[PC];
	PC++;

	// Execution Cycle
	switch (IR.op) {
		case FCH:	//treated as NOOP, (fetch already executed, TODO there may be a better way to do this, but the FETCH opcode isn't in the design (traditionally a hidden opcode))
			break;
		case LIT:	//push literal IR.m onto stack
			SP++;
			stack[SP] = IR.m;
			break;
		case OPR:	//operate on stack, depending on literal in instruction register, for operations, see operateOnStack()
			operateOnStack();
			break;
		case LOD:	//push value at offset IR.m from IR.l lexicographical levels down onto stack
			SP++;
			stack[SP] = stack[base(IR.l, BP) + IR.m];
			break;
		case STO:	//pop value off stack and store at offset IR.m from IR.l lexicographical levels down
			stack[base(IR.l, BP) + IR.m] = stack[SP];
			SP--;
			break;
		case CAL:	//call procedure at code index IR.m (assuming in this case that the stack pointer will be properly incremented after this call is made to allocate local variables?)
			stack[SP + 1] = base(IR.l, BP);	//static link
			stack[SP + 2] = BP;				//dynamic link
			stack[SP + 3] = PC;				//return address
			BP = SP + 1;
			PC = IR.m;
			break;
		case INC:	//increment stack pointer by IR.m
			SP += IR.m;
			break;
		case JMP:	//jump to instruction IR.m
			PC = IR.m;
			break;
		case JPC:	//jump to instruction IR.m IF top stack element == 0
			if(stack[SP] == 0){
				PC = IR.m;
			}
			SP--;
			break;
		case SIO:
			standardIOOperation();
			break;
		case 10: break;	//suppressed, not technically a valid operation based on conversation with instructor
		default:
			printf("ERROR: Invalid OPCODE %d! Line: %d\n", IR.op, PC);
			exit(ERROR_INVALID_OPCODE);
	}
}

void operateOnStack() {	// do operation IR.m on stack
	switch (IR.m) {
		case RET:
			SP = BP - 1;
			PC = stack[SP + 3];
			BP = stack[SP + 2];
			break;
		case NEG:
			stack[SP] *= -1;
			break;
		case ADD:
			SP--;
			stack [SP] = stack[SP] + stack[SP + 1];
			break;
		case SUB:
			SP--;
			stack[SP] = stack[SP] - stack[SP + 1];
			break;
		case MUL:
			SP--;
			stack[SP] = stack[SP] * stack[SP + 1];
			break;
		case DIV:
			SP--;
			stack[SP] = stack[SP] / stack[SP + 1];
			break;
		case ODD:
			stack[SP] %= 2;
			break;
		case MOD:
			SP--;
			stack[SP] %= stack[SP + 1];
			break;
		case EQL:
			SP--;
			stack[SP] = stack[SP] == stack[SP+1];
			break;
		case NEQ:
			SP--;
			stack[SP] = stack[SP] != stack[SP + 1];
			break;
		case LSS:
			SP--;
			stack[SP] = stack[SP] < stack[SP + 1];
			break;
		case LEQ:
			SP--;
			stack[SP] = stack[SP] <= stack[SP + 1];
			break;
		case GTR:
			SP--;
			stack[SP] = stack[SP] > stack[SP + 1];
			break;
		case GEQ:
			SP--;
			stack[SP] = stack[SP] >= stack[SP + 1];
			break;
		default:
			printf("ERROR: Invalid STACK_OPERATION on line %d: %d cannot be resolved to an operation.\n", PC, IR.m);
			printStack();
			printf("\n");
			exit(ERROR_INVALID_STACK_OPERATION);
	}
}

void standardIOOperation() {

        switch (IR.m) {
                case 0: //for the purposes of testing the factorial code, which was based on a slightly different instruction set, this instruction will do the same as SOT
                case SOT:       // pop the top element of the stack off and display it to standard out
                        printf("\t    The value of %s is %d\n\t\t", Osymbol.name, stack[SP]);
                        SP--;
                        break;
                case SIN:       // push the first integer to be entered from standard input onto the stack, keep retrying till good data is gotten
                        SP++;
                        int i;
			printf("\t    Enter value of %s: ", Isymbol.name);

			while (!scanf("%d", &i));
                        printf("\t\t ");
			stack[SP] = i;
			updateTable(Isymbol);
                        break;
                default:
                        printf("ERROR: Invalid SIO_OPERATION on line %d: %d cannot be resolved to a standard IO operation.\n", PC, IR.m);
                        printStack();
                        printf("\n");
                        exit(ERROR_INVALID_IO_OPERATION);
        }
}

// LEXICAL ANALYSER STARTS HERE
// Prints the number of errors and returns TRUE if errors where found and False otherwise.
int printErrors(){
	int i, hasErrors = FALSE;

	for( i = 0; i < ERRMAX; i++){
		if(errors[i] > 0)
			hasErrors = TRUE;
	}

	for( i = 0; i < ERRMAX; i++){
		if(i == VARERR){
			printf("\n\nThere where %d Variable(s) errors\n", errors[i]);
		}
		else if(i == NUMERR){
			printf("There where %d Number(s) that where too long\n", errors[i]);
		}
		else if(i == VARLENERR){
			printf("There where %d Variable(s) that where too long\n", errors[i]);
		}
		else if(i == SYMBERR){
			printf("There where %d foreign symbole(s)\n", errors[i]);
		}
	}

	if(hasErrors == TRUE)
		return TRUE;
	else
		return FALSE;
}

// Prints the lexeme list and symbolic representation of tokens
void printLexemeList(){
	int i;
	printf("\nLexeme List\n");
	for(i = 0; i < h-1; i++){
		if(nameRec[i].hasValue == TRUE)
			printf("%d %d ", nameRec[i].kind, nameRec[i].value);
		else if(nameRec[i].kind == identsym)
			printf("%d %s ", nameRec[i].kind, nameRec[i].name);
		else
			printf("%d ", nameRec[i].kind);
	}
	printf("\n");

	i = 0;
	printf("\nSymbolic Representation\n");

	for( i = 0; i < h-1; i ++){
		if(nameRec[i].hasValue == TRUE)
			printf("%s.%d\n", symbolesName[nameRec[i].kind], nameRec[i].value);
		else if(nameRec[i].kind == identsym)
			printf("%s.%s\n", symbolesName[identsym], nameRec[i].name);
		else
			printf("%s\n", symbolesName[nameRec[i].kind]);
	}

}

// Prints the lexeme table
void printLexemeTable(){
	int i;
	printf("\n   Lexeme  token type\n");

	for( i = 0; i < h-1; i++){
		if(nameRec[i].hasValue == TRUE)
			printf("%9d %8d\n", nameRec[i].value, nameRec[i].kind);
		else
			printf("%9s %8d\n", nameRec[i].name, nameRec[i].kind);
	}

}

// Initializes the error array and initializes the counters
void initialize(){
        int i;

        for( i = 0; i < ERRMAX; i ++){
                errors[i]= FALSE;
        }

        charCount = 0;
        line[0] = '\0';
}

// retrieves next character
void nextCharacter(){

        if(line[charCount] == '\0'){
			int i = 0,
				j = 0;
			char c;
			char newLine[sizeof(line)];
			char* fetchLine = fgets(newLine, sizeof(line), ifp);
			lineCount++;
			charCount = 0;

			while(c = newLine[i++]){
				if(isprint(c))
					line[j++] = c;
				// If a tab is encountered is spaces it out accordingly with the printed code
				else if( c == '\t'){
					line[j++] = ' ';
					while (j % 8 != 1)
						line[j++] = ' ';
				}
			}
			line[j++] = '\n';
			line[j] = '\0';

			if(!feof(ifp)){
				printf("%s", line);
			}
	}

		currChar = line[charCount++];
}

// This function will read through white space and comments
// when encountered
void skip(){

        while(currChar <= ' '){
			nextCharacter();
		}
}

// Skips over comments
void skipComment(){
	int i = FALSE;
	if(currChar == '/'){
		previous = currChar;
		nextCharacter();
		if(currChar == '*'){
			do{
				nextCharacter();
				if(currChar == '*'){
					nextCharacter();
					if(currChar == '/'){
						i = TRUE;
						nextCharacter();
						skip();
					}
				}
			}while(i == FALSE);
		}
		else{
			currChar = previous;
			charCount--;
		}
	}

}

// Turns characters into their respective tokens
void getToken(){
	int i, j, k, l, done = FALSE, found;
	char id[CMAX + 1];
	struct token currToken;
	int start = 0;
	int spdone = 0;

	while(!done){
		skip();
		skipComment();

		nameRec[h].hasValue = FALSE;

		// if the current character is a letter
		if(isalpha(currChar)){
			k = 0;
			// Stores the name of the current word into token struct
			do{
				if(k < CMAX)
					nameRec[h].name[k++] = currChar;
				nextCharacter();
				} while(isalnum(currChar));

			// Sets terminating character
			nameRec[h].name[k] = '\0';
			found = 0;

			// Match to see if it's a reserved word
			for(i = 0; i < numReservWords; i++){
				if(strcmp(nameRec[h].name, reservedWords[i])==0){
					nameRec[h].kind = i + 21;
					found = 1;
					done = TRUE;
					}
				else if(strcmp(nameRec[h].name, "odd") == 0){
					nameRec[h].kind = oddsym;
					found = 1;
					done = TRUE;;
				}
				else if(strcmp(nameRec[h].name, "procedure") == 0){
					nameRec[h].kind = procsym;
					found = 1;
					done = TRUE;
				}
			}

			// IT's an identifier
			if(found == 0){
				if(strlen(nameRec[h].name) < CMAX){
					nameRec[h].kind = identsym;
					done = TRUE;
				}
				else{
					errors[VARLENERR]++;
				}
			}
		}

		// Check if intiger or special character or invalid variable
		else{
			// check integer
			invalidVar = FALSE;
			k = 0;
			start = 0;
			if(isdigit(currChar)){
				do{
					if(isdigit(currChar) && invalidVar == FALSE){
						start = start * 10 + (currChar - '0');
						k++;
					}
					else
						start = 0;

					nextCharacter();

					if(isalpha(currChar)){
						do{
							nextCharacter();
							invalidVar = TRUE;
						}while(isdigit(currChar) || isalpha(currChar));
					}
				}while(isdigit(currChar));

				if(invalidVar == FALSE){
					if(k > DMAX){
						errors[NUMERR]++;

						nameRec[h].value = 0;
					}
					else{
						nameRec[h].value = start;
						nameRec[h].kind = numbersym;
						nameRec[h].hasValue = TRUE;
						done = TRUE;
					}
				}
				else{
					errors[VARERR]++;
				}
			}
			// check special character
			else if (invalidVar == FALSE){
				switch(currChar){
					case '+':
						nameRec[h].kind = plussym;
						strcpy(nameRec[h].name, "+");
						nextCharacter();
						done = TRUE;
						break;
					case '-':
						nameRec[h].kind = minussym;
						strcpy(nameRec[h].name, "-");
						nextCharacter();
						done = TRUE;
						break;
					case '*':
						nameRec[h].kind = multsym;
						strcpy(nameRec[h].name, "*");
						nextCharacter();
						done = TRUE;
						break;
					case '/':
						nameRec[h].kind = slashsym;
						strcpy(nameRec[h].name, "/");
						nextCharacter();
						done = TRUE;
						break;
					case '(':
						nameRec[h].kind = lparentsym;
						strcpy(nameRec[h].name, "(");
						nextCharacter();
						done = TRUE;
						break;
					case ')':
						nameRec[h].kind = rparentsym;
						strcpy(nameRec[h].name, ")");
						nextCharacter();
						done = TRUE;
						break;
					case '=':
						nameRec[h].kind = eqlsym;
						strcpy(nameRec[h].name, "=");
						nextCharacter();
						done = TRUE;
						break;
					case ',':
						nameRec[h].kind = commasym;
						strcpy(nameRec[h].name, ",");
						nextCharacter();
						done = TRUE;
						break;
					case '.':
						nameRec[h].kind = periodsym;
						strcpy(nameRec[h].name, ".");
						nextCharacter();
						done = TRUE;
						break;
					case '<':
						nextCharacter();
						if(currChar == '='){
							nameRec[h].kind = leqsym;
							strcpy(nameRec[h].name, "<=");
							nextCharacter();
						}
						else{
							nameRec[h].kind = lessym;
							strcpy(nameRec[h].name, "<");
						}
						done = TRUE;
						break;
					case '>':
						nextCharacter();
						if(currChar == '='){
							nameRec[h].kind = geqsym;
							strcpy(nameRec[h].name, ">=");
							nextCharacter();
						}
						else{
							nameRec[h].kind = gtrsym;
							strcpy(nameRec[h].name, ">");
						}
						done = TRUE;
						break;
					case ';':
						nameRec[h].kind = semicolonsym;
						strcpy(nameRec[h].name, ";");
						nextCharacter();
						done = TRUE;
						break;
					case '[':
						nameRec[h].kind = lparentsym;
						strcpy(nameRec[h].name, "[");
						nextCharacter();
						done = TRUE;
						break;
					case ']':
						nameRec[h].kind = rparentsym;
						strcpy(nameRec[h].name, "]");
						nextCharacter();
						done = TRUE;
						break;
					case ':':
						nextCharacter();

						if(currChar == '='){
							nameRec[h].kind = becomessym;
							strcpy(nameRec[h].name, ":=");
							nextCharacter();
							done = TRUE;
						}
						else
							errors[SYMBERR]++;
						break;
					case '!':

						nextCharacter();
						if(currChar == '='){
							nameRec[h].kind = neqsym;
							strcpy(nameRec[h].name, "!=");
							nextCharacter();
							done = TRUE;
						}
						else
							errors[SYMBERR]++;
						break;
					default:
						if(currChar != ' ' || currChar != '\n' || currChar != '\t')
							errors[SYMBERR]++;
						nextCharacter();
				}
			}
		}
		if(done)
			h++;
		}
}

// PARSER BEGINS HERE
void Program(){
	nextToken();
	Block();
	if(currToken.kind != periodsym)
		printParserErrors(9);
	else{
		printf("\nNo errors, program is syntactically correct\n\n");
	}
}

void Block(){
	int ctemp, 	// Holds the code index
	    currM;	// Holds the number of variables

	// If this is the first lexigraphical level
	if(first){
		lexilvl = 0;
		first = FALSE;
	}
	else
		lexilvl++;

	// If token kind is a constant
	if(currToken.kind == constsym){
		do{
			// Copy type into symbol struct
			symbol_table[numSymbols].kind= CONST;

			nextToken();
			if(currToken.kind != identsym)
				printParserErrors(4);

			// Copy name into symbol struct
			strcpy(symbol_table[numSymbols].name, currToken.name);


			nextToken();
			if(currToken.kind != eqlsym)
				printParserErrors(3);

			nextToken();
			if(currToken.kind != numbersym)
				printParserErrors(2);

			// Copy value into symbol strut
			symbol_table[numSymbols].val = currToken.value;

			// Increase number of symbols
			numSymbols++;

			nextToken();

		}while(currToken.kind == commasym);

		if(currToken.kind != semicolonsym)
			printParserErrors(5);

		// Update current Line
		currLine++;
		nextToken();
	}

	// Make room for DL/ SL/ RA
	m = 3;
	currM = 3;

	// If token is a variable (INT was used instead of VAR in lexical analyzer definition)
	if(currToken.kind == intsym){
		do{
			// Copy type into syStruct
			symbol_table[numSymbols].kind = INT;

			nextToken();

			if(currToken.kind != identsym)
				printParserErrors(4);

			// Copy name, level, and adress into  syStruct
			strcpy(symbol_table[numSymbols].name, currToken.name);
			symbol_table[numSymbols].level = lexilvl;
			symbol_table[numSymbols].addr = m;

			// Increase the number of variables
			m++;

			// Increase the number of symbols
			numSymbols++;

			nextToken();

		}while(currToken.kind == commasym);

		if(currToken.kind != semicolonsym)
			printParserErrors(5);

		// Update line
		currLine++;

		nextToken();

		currM = m;
	}

	// If token is a procedure
	if(currToken.kind == procsym){
		ctemp = cx;
		emit(JMP, 0, 0);
	}

	else{
		emit(INC, 0 ,m);
	}

	// If token is a procedure
	while(currToken.kind == procsym){

		// Store symbol kind into sy Struct
		symbol_table[numSymbols].kind = PROC;

		emit(JMP, 0, cx+1);

		nextToken();

		if(currToken.kind != identsym){
			printParserErrors(6);
		}

		// Store name, level, and adress into sy Struct
		strcpy(symbol_table[numSymbols].name, currToken.name);
		symbol_table[numSymbols].level = lexilvl;
		symbol_table[numSymbols].addr = cx;

		numSymbols++;

		nextToken();

		if(currToken.kind != semicolonsym)
			printParserErrors(5);

		nextToken();

		Block();

		if(currToken.kind != semicolonsym)
			printParserErrors(5);

		// Store where to jump in first jump command
		codeStack[ctemp].m = cx;

		emit(INC, 0, currM);
		currLine++;
		nextToken();

	}


	if(currToken.kind != beginsym)
		printParserErrors(7);

	currLine++;
	Statement();

	emit(OPR, 0, RET);

	// Decrease the lexigraphical level
	lexilvl--;

	// Remove variables from previous lexigraphical level
	removeVariables();
}

void Statement(){
	int currM;			// Holds an adress value
	int jTemp; 			// Holds an adress value for a jump command
	symbol locTempSymbol;		// Holds a symbol


	if(currToken.kind == identsym){
		// Checks if variable is in the symbol table and is the correct king
		CIT = checkIdentType();
		if(CIT == CONST || CIT == PROC)
			printParserErrors(12);
		if(CIT == 0)
			printParserErrors(11);

		// Stores current symbol
		locTempSymbol = getSymbol();

		nextToken();

		if(currToken.kind != becomessym){
			if(currToken.kind == eqlsym)
				printParserErrors(1);
			printParserErrors(13);
		}
		nextToken();

		Expression();

		// Store symbol and update symbol table
		if(lexilvl != 0)
			emit(STO, abs(lexilvl - locTempSymbol.level), locTempSymbol.addr);
		else
			emit(STO, lexilvl, locTempSymbol.addr);

		updateTable(locTempSymbol);

	}
	else if(currToken.kind == callsym){
		nextToken();

		if(currToken.kind != identsym)
			printParserErrors(14);

		// check if indentifier is on symbol table and the correct type of symbol
		CIT = checkIdentType();

		if(CIT == CONST || CIT == INT)
			printParserErrors(15);
		if(CIT == 0)
			printParserErrors(11);

		tempSymbol = getSymbol();

		if(lexilvl != 0)
			emit(CAL, abs(lexilvl - tempSymbol.level), tempSymbol.addr);
		else
			emit(CAL, tempSymbol.level, tempSymbol.addr);

		currLine++;
		nextToken();
	}
	else if(currToken.kind == beginsym){
		nextToken();
		Statement();

		while(currToken.kind == semicolonsym){
			currLine++;
			nextToken();
			Statement();
		}
		if(currToken.kind != endsym)
			printParserErrors(8);
		currLine++;
		nextToken();
	}
	else if(currToken.kind == ifsym){
		nextToken();
		Condition();

		if(currToken.kind != thensym)
			printParserErrors(16);
		else
			nextToken();

		currLine++;
		ctemp = cx;
		emit(JPC, 0, 0);
		Statement();

		jTemp = cx;
		emit(JMP, 0, 0);

		codeStack[ctemp].m = cx;
		nextToken();

		if(currToken.kind == elsesym){
			currLine++;
			nextToken();
			Statement();
			codeStack[jTemp].m = cx;
		}
		else{
			currTokenLocation--;
			currTokenLocation--;
			currToken = nameRec[currTokenLocation];
			codeStack[jTemp].m = cx;
		}

	}
	else if(currToken.kind == whilesym){
		int currM = cx;

		nextToken();

		Condition();

		if(currToken.kind != dosym)
			printParserErrors(18);

		currLine++;
		nextToken();
		ctemp = cx;

		emit(JPC, 0, 0);

		Statement();

		emit(JMP, 0, currM);

		codeStack[ctemp].m = cx;
	}
	else if(currToken.kind == insym){
		nextToken();

		if(currToken.kind != identsym)
			printParserErrors(26);

		Isymbol = getSymbol();

		emit(SIO, 0, SIN);

		if(lexilvl != 0)
			emit(STO, abs(lexilvl - Isymbol.level), Isymbol.addr);
		else
			emit(STO, lexilvl, Isymbol.addr);

		nextToken();

		currLine++;
	}
	else if(currToken.kind == outsym){
		nextToken();

		if(currToken.kind != identsym)
			printParserErrors(27);

		Osymbol = getSymbol();

		emit(LOD, 0, Osymbol.addr);
		emit(SIO, 0, SOT);

		nextToken();
		currLine++;
	}
}

void Condition(){
	int relatop;

	if(currToken.kind == oddsym){
		relatop = currToken.kind;
		nextToken();
		Expression();
		if(relatop == oddsym)
			emit(OPR, 0, ODD);
	}
	else{
		Expression();

		if(currToken.kind != gtrsym && currToken.kind != geqsym &&
		   currToken.kind != lessym && currToken.kind != leqsym &&
		   currToken.kind != eqlsym && currToken.kind != neqsym)
			printParserErrors(20);

		else{
			if(currToken.kind == gtrsym)
				relatop = gtrsym;
			else if(currToken.kind == geqsym)
				relatop = geqsym;
			else if(currToken.kind == lessym)
				relatop = lessym;
			else if(currToken.kind == leqsym)
				relatop = leqsym;
			else if(currToken.kind == eqlsym)
				relatop = eqlsym;
			else if(currToken.kind == neqsym)
				relatop = neqsym;
		}
		nextToken();
		Expression();

		if(relatop == gtrsym)
			emit(OPR, 0, GTR);
		else if(relatop == geqsym)
			emit(OPR, 0, GEQ);
		else if(relatop == lessym)
			emit(OPR, 0, LSS);
		else if(relatop== leqsym)
			emit(OPR, 0, LEQ);
		else if(relatop == eqlsym)
			emit(OPR, 0, EQL);
		else if(relatop == neqsym)
			emit(OPR, 0, NEQ);
	}
}

void Expression(){
	int addop;
	if(currToken.kind == plussym || currToken.kind == minussym){
		addop = currToken.kind;
		nextToken();
		Term();
		if(addop == minussym)
			emit(OPR, 0, NEG);// negate
	}
	else
		Term();

	while(currToken.kind == plussym || currToken.kind == minussym){
		addop = currToken.kind;
		nextToken();
		Term();
		if(addop == plussym)
			emit(OPR, 0, ADD);	// addition
		else
			emit(OPR, 0, SUB);	// subtraction
	}
}

void Term () {
	int mulop;
	Factor();

	while(currToken.kind == multsym || currToken.kind == slashsym){
		mulop = currToken.kind;

		nextToken();

		Factor();

		if(mulop == multsym)
			emit(OPR, 0, MUL);	// multiply
		else
			emit(OPR, 0, DIV);	// divide
	}
}

void Factor(){
	if(currToken.kind == identsym){
		CIT = checkIdentType();

		if(CIT == CONST)
			emit(LIT, 0, tempSymbol.val);

		else if(CIT == INT){
			if(lexilvl != 0)
				emit(LOD, abs(lexilvl - tempSymbol.level), tempSymbol.addr);
			else
				emit(LOD, tempSymbol.level, tempSymbol.addr);
		}

		else if(CIT == PROC)
			printParserErrors(21);

		else if(CIT == 0)
			printParserErrors(11);

		nextToken();
	}
	else if(currToken.kind == numbersym){
		emit(LIT, 0, currToken.value);
		nextToken();
	}
	else if(currToken.kind == lparentsym){
		nextToken();
		Expression();

		if(currToken.kind != rparentsym)
			printParserErrors(22);

		nextToken();
	}
	else
		printParserErrors(23);
}

void printParserErrors(int errorNum){
	printf("\n");
	printf("Error found on line %d: ", currLine);

	switch(errorNum){
		case 1: printf("Use = instead of :=.\n");
			break;
		case 2: printf("= must be followed by a number.\n");
			break;
		case 3: printf("Identifier must be followed by =.\n");
			break;
		case 4: printf("const, var, procedure must be followed by identifier.\n");
			break;
		case 5: printf("Semicolon or comma missing.\n");
			break;
		case 6: printf("Incorrect symbol after procedure declaration.\n");
			break;
		case 7: printf("Statement expected.\n");
			break;
		case 8: printf("Incorrect symbol after statement part in block.\n");
			break;
		case 9: printf("Period expected.\n");
			break;
		case 10: printf("Semicolon between statements missing.\n");
			break;
		case 11: printf("Undeclared identifier.\n");
			break;
		case 12: printf("Assignment to constant or procedure is not allowed.\n");
			break;
		case 13: printf("Assignment operator expected.\n");
			break;
		case 14: printf("call must be followed by an identifier.\n");
			break;
		case 15: printf("Call of a constant or variable is meaningless.\n");
			break;
		case 16: printf("then expected.\n");
			break;
		case 17: printf("Semicolon or } expected.\n");
			break;
		case 18: printf("do expected.\n");
			break;
		case 19: printf("Incorrect symbol following statement.\n");
			break;
		case 20: printf("Relational operation expected.\n");
			break;
		case 21: printf("Expression must not contain a procedure identifier.\n");
			break;
		case 22: printf("Right parenthesis missing.\n");
			break;
		case 23: printf("The preceding factor cannot being with this symbol.\n");
			break;
		case 24: printf("An expression cannot begin with this symbol.\n");
			break;
		case 25: printf("This number is too large.\n");
			break;
		case 26: printf("Identifier expected after 'in' command.\n");
			break;
		case 27: printf("Identifier expected after 'out' command.\n");
		default:
			break;
	}
	exit(1);
}

// remove variables on previous lexigraphical levels
void removeVariables(){
	int i;
	int temp = 0;
	for(i = 0; i < numSymbols; i++){
		if(symbol_table[i].level > lexilvl)
			temp++;
	}
	numSymbols-=temp;

}

// updates value on symbol table
void updateTable(symbol temp){
	int i;
	for(i = numSymbols; i >= 0; i--){
		if(strcmp(temp.name, symbol_table[i].name) == 0){
			symbol_table[i].val = temp.val;
		}
	}
}

// retrieves the current symbol
symbol getSymbol(){
	int i;
	for(i = numSymbols; i >= 0; i--){
		if(strcmp(currToken.name, symbol_table[i].name) == 0){
			return symbol_table[i];
		}
	}
}

// Traverses the token array
void nextToken(){
	currToken = nameRec[currTokenLocation];
	currTokenLocation++;
}

// Checks the symbol type
int checkIdentType(){
	int i;
	for(i = numSymbols; i >= 0; i--){
		if(strcmp(currToken.name, symbol_table[i].name) == 0){
			tempSymbol = symbol_table[i];
			return symbol_table[i].kind;
		}
	}
}

void emit(int op, int l, int m){
	if (cx > CODE_SIZE)
		printParserErrors(25);
	else{
		codeStack[cx].op = op;
		codeStack[cx].l = l;
		codeStack[cx].m = m;
		cx++;
	}
}

void printSymbolTable(){
	int i;
	printf("\nSymbolTable\n");
	for(i = 0; i < numSymbols; i++){
		printf("Kind %d, Name %s, Level %d\n", symbol_table[i].kind, symbol_table[i].name, symbol_table[i].level);
	}
}

void printGeneratedCode(){
	int i;
	printf("\nGenerated Code\n");
	for(i = 0; i < cx; i++){
		printf("%s %d %d\n", instructions[codeStack[i].op], codeStack[i].l, codeStack[i].m);
	}
	printf("\n");
}
