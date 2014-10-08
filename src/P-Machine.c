/*
Anthony Dinh
COP 3402 : System Software
September 8th 2014
Assignment 1 : P-Machine (VM)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3


typedef struct instruction{
    int op; // opcode
    int l;  // L
    int m;   // M
}instruction;


static int stack[MAX_STACK_HEIGHT];
static int bar[MAX_STACK_HEIGHT];
static instruction code[MAX_CODE_LENGTH];

static instruction IR;
static int BP;
static int SP;
static int PC;
static int OLD_PC;
static int HALT;

//Fetches the next instruction from the code array
void fetch()
{

    IR = code[PC];
    OLD_PC = PC;
    PC++;
}

//Finds the base L levels down
int base(int l, int base) // l stands for L in the instruction format
{
    int bl; //find base L levels down
    bl = base;
    while (l > 0)
    {
        bl = stack[bl + 1];
        l--;
    }
    return bl;
}

void OPRFinder()
{

    switch (IR.m){
        //Ret
        case 0:
            SP = BP -1;
            PC = stack[SP + 4];
            BP = stack[SP + 3];
            break;
        //Negation
        case 1:
            stack[SP] = -stack[SP];
            break;
        //Addition
        case 2:
            SP--;
            stack[SP] = stack[SP] + stack[SP + 1];
            break;
        //Subtraction
        case 3:
            SP--;
            stack[SP] = stack[SP] - stack[SP + 1];
            break;
        //Multiplication
        case 4:
            SP--;
            stack[SP] = stack[SP] * stack[SP + 1];
            break;
        //Division
        case 5:
            SP--;
            stack[SP] = stack[SP] / stack[SP + 1];
            break;
        //Mod 2
        case 6:
            stack[SP] = stack[SP] % 2;
            break;
        //Mod
        case 7:
            SP--;
            stack[SP] = stack[SP] % stack[SP + 1];
            break;
        //Equality
        case 8:
            SP--;
            stack[SP] = (stack[SP] == stack[SP + 1]);
            break;
        //Not Equal
        case 9:
            SP--;
            stack[SP] = (stack[SP] != stack[SP + 1]);
            break;
        //Less than
        case 10:
            SP--;
            stack[SP] = (stack[SP] < stack[SP + 1]);
            break;
        //Less than equal
        case 11:
            SP--;
            stack[SP] = (stack[SP] <= stack[SP + 1]);
            break;
        //Greater than
        case 12:
            SP--;
            stack[SP] = (stack[SP] > stack[SP + 1]);
            break;
        //Greater than equal to
        case 13:
            SP--;
            stack[SP] = (stack[SP] >= stack[SP + 1]);
            break;
        default:
            printf("Something broke in the OPR finder");
            exit(1);



    }

}


//Returns the string version of the opcode
const char * opcodeFinder(int opcode)
{
    switch (opcode){
        //Push Literal
        case 1:
            return "LIT";
            break;
        //Look at M to figure out operation
        case 2:
            return "OPR";
            break;
        //Load
        case 3:
            return "LOD";
            break;
        //Store
        case 4:
            return "STO";
            break;
        //Create new activation record
        case 5:
            return "CAL";
            break;
        //Increments stack by M
        case 6:
            return "INC";
            break;
        //Jump
        case 7:
            return "JMP";
            break;
        //Jump Condition
        case 8:
            return "JPC";
            break;
        //Writes top stack to screen
        case 9:
        //Reads input from user and stores to top of stack
        case 10:
        //Halts the Program
        case 11:
            return "SIO";
            break;
        default:
            printf("Something broke, you should not get here");
            exit(1);


    }
}

//Outputs the current variables and  etc
void outputStack(FILE *ofp, FILE *ofp2, int printStack)
{
    int i;

    //Prints everything except the stack
    fprintf(ofp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t", OLD_PC, opcodeFinder(IR.op), IR.l, IR.m, PC, BP, SP );
    fprintf(ofp2, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t", OLD_PC, opcodeFinder(IR.op), IR.l, IR.m, PC, BP, SP );

    if (printStack)
        printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t", OLD_PC, opcodeFinder(IR.op), IR.l, IR.m, PC, BP, SP );

    //Does not print the stack if SP == 0
    if (SP != 0){

        //Prints the bar separating the AR
        for (i = 1; i<=SP; i++){

            if (bar[i] && i != 1){

                fprintf(ofp, "| ");
                fprintf(ofp2, "| ");

                if (printStack)
                    printf("| ");
            }

            fprintf(ofp, "%d ", stack[i]);
            fprintf(ofp2, "%d ", stack[i]);

            if (printStack)
                printf("%d ", stack[i]);
        }
    }

    fprintf(ofp, "\n");
    fprintf(ofp2, "\n");

    if (printStack)
        printf("\n");
}

void execute()
{
    switch (IR.op){
        //Pushes literal onto the stack
        case 1:
            SP++;
            stack[SP] = IR.m;
            break;
        //Operation check M for final
        case 2:
            OPRFinder();
            break;
        //Loads values onto stack
        case 3:
            SP++;
            stack[SP] = stack[base(IR.l, BP) + IR.m];
            break;
        //Stores Value
        case 4:
            stack[base(IR.l,BP) + IR.m] = stack[SP];
            SP--;
            break;
        //Generates new activation record
        case 5:
            stack[SP + 1] = 0;
            stack[SP + 2] = base(IR.l, BP);
            stack[SP + 3] = BP;
            stack[SP + 4] = PC;
            BP = SP + 1;
            PC = IR.m;
            break;
        //Allocates M Locals onto the stack
        case 6:
            bar[SP + 1] = 1;
            SP = SP + IR.m;
            break;
        //Jump
        case 7:
            PC = IR.m;
            break;
        //Jumps on a condition
        case 8:
            if (stack[SP] == 0)
                PC = IR.m;
            SP--;
            break;
        //Prints the topmost stack element to the screen
        case 9:
            printf("%d\n", stack[SP]);
            SP--;
            break;
        //Reads input from user and stores at the top of the stack
        case 10:
            SP++;
            scanf("%d",&stack[SP]);
            break;
        //Exits the Program
        case 11:
            HALT = 0;
            break;
        default:
            printf("Something went wrong with the execution function\n");
            printf("%d",IR.op);
            exit(1);



    }

}



int main(int argc, char *argv[])
{

    int printStack = 0;

    int i = 0;
    int length = 0;

    //Checks to see if the number of arguments is correct
    if (argc > 2){

        printf("Invalid number of arguments: %d", argc);

        exit(1);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-v") == 0){

            printStack = 1;
            continue;
        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );
            exit(1);


        }

    }

    

    i = 0;
  
    //Program ends when HALT is false
    HALT = 1;

    BP = 1;
    SP = 0;
    PC = 0;
    OLD_PC = 0;

    //Checks to see if the file exists
    if (!(access( "mcode.txt", F_OK ) != -1)){
        printf("mcode.txt does not exist in the current directory\n");
        exit(1);
    }

    //Open the input file, and the output file
    FILE *ifp, *ofp, *ofp2;
    ifp = fopen("mcode.txt", "r");
    ofp = fopen("stacktrace.txt", "w");
    ofp2 = fopen("output.txt", "a");

    while (1){

        fscanf(ifp, "%d %d %d", &code[i].op, &code[i].l, &code[i].m);

         if (feof(ifp))
            break;

        i++;

    }

    i = 0;
    while (code[i].op != 0)
        i++;

    length = i;

    fprintf(ofp, "line\tOP\tL\tM\n");
    fprintf(ofp2, "line\tOP\tL\tM\n");

    

    if (printStack)
        printf("line\tOP\tL\tM\n");

    for (i = 0; i < length; i++){
        
        fprintf(ofp, "%d\t%s\t%d\t%d\n", i, opcodeFinder(code[i].op), code[i].l, code[i].m);
        fprintf(ofp2, "%d\t%s\t%d\t%d\n", i, opcodeFinder(code[i].op), code[i].l, code[i].m );

        if (printStack)
            printf("%d\t%s\t%d\t%d\n", i, opcodeFinder(code[i].op), code[i].l, code[i].m);

    }

    fprintf(ofp, "\t\t\t\tPC\tBP\tSP\tstack\n");
    fprintf(ofp, "Initial values\t\t\t%d\t%d\t%d\n", PC, BP, SP );

    fprintf(ofp2, "\t\t\t\tPC\tBP\tSP\tstack\n");
    fprintf(ofp2, "Initial values\t\t\t%d\t%d\t%d\n", PC, BP, SP );

    if (printStack){

        printf("\t\t\t\tPC\tBP\tSP\tstack\n");
        printf("Initial values\t\t\t%d\t%d\t%d\n", PC, BP, SP );
    }


    //We close the file now that we are done using it
    fclose(ifp);

    //Sets the stack to 0
    memset(stack,0,sizeof(int)*MAX_STACK_HEIGHT);
    memset(bar,0,sizeof(int)*MAX_STACK_HEIGHT);


    //THe Program runs while the base pointer is not equal to 0 and the Halt flag is true
    while(BP!=0 && HALT){

        fetch();
        execute();
        outputStack(ofp, ofp2, printStack);
     
    }



    fclose(ofp);
    fclose(ofp2);

    return 0;
  
}
