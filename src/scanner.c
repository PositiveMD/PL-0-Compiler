
/*
Anthony Dinh
COP 3402 : System Software
September 21th 2014
Assignment 2 : Lexical Analyzer
*/

#include "CompilerDriver.h"


typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} token_types;

int main(int argc, char *argv[])
{

    //Checks to see if the file exists
    if (!(access( "input.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(0);
    }

    FILE *ifp, *ofp1, *ofp2, *ofp3;
    ifp = fopen("input.txt", "r");
    ofp1 = fopen("cleaninput.txt", "w");
    ofp2 = fopen("lexemetable.txt", "w");
    ofp3 = fopen("lexemelist.txt", "w");

    return 0;
}
