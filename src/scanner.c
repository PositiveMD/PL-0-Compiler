
/*
Anthony Dinh
COP 3402 : System Software
September 21th 2014
Assignment 2 : Lexical Analyzer
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUM_LENGTH 5
#define MAX_IDENT_LENGTH 11

//The enums with all the token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym,
    slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym,
    lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym,
    beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym,
    varsym, procsym, writesym, readsym, elsesym
} token_types;

//Removes the comments from the input file and moves it the a commentless input file
void removeComments(FILE *ifp, FILE *ofp1, FILE *ofp4)
{
    int temp, temp2;

    while ( (temp=getc(ifp)) != EOF){

        if (temp == '/')
        {
            temp = getc(ifp);

            if ( temp != '*'){
                putc('/', ofp1);
                putc(temp, ofp1);

                putc('/', ofp4);
                putc(temp, ofp4);
            }

            else {

                fprintf(ofp1, " ");
                fprintf(ofp4, " ");
                do{
                    temp2 = temp;
                    temp = getc(ifp);
                } while (temp != '/' || temp2 != '*');
            }
        }

        else{
            putc(temp, ofp1);
            putc(temp, ofp4);
        }
    }
    fprintf(ofp4, "\n");
    fclose(ifp);
    fclose (ofp1);

}

//Takes the clean input file and converts it into tokens
void convertToToken(FILE *ifp, FILE *ofp2, FILE *ofp3, FILE *ofp4, int printLex){

    int temp;
    int count;
    int hasDigit;
    int value;
    int i;

    //Add space for the null terminator
    char charBuffer[MAX_IDENT_LENGTH + 1];



    fprintf(ofp2, "lexeme\ttoken type\n");
    

    while ((temp = getc(ifp)) != EOF){


        //Resets all the variables at the beginning of the loop
        count = 0;
        hasDigit = 0;
        value = 0;
        memset(charBuffer, 0 ,sizeof(char)*MAX_IDENT_LENGTH + 1);

        //The character may be a variable or reserved word
        if (isalpha(temp)){

            do{

                //If the characters being read in have a digit, it cannnot be a reserved word
                if (isdigit(temp))
                    hasDigit = 1;

                charBuffer[count] = temp;
                count++;

                if (count > MAX_IDENT_LENGTH){

                    printf("The name is too long");
                    exit (0);
                }

            //Runs the loops while the input is alphanumeric
            }while (isalnum( temp = getc(ifp)));

            //Does not contain a digit, so it is not a reserved word and can print it.
            if (hasDigit){

                fprintf(ofp2, "%s\t%d\n", charBuffer, identsym);
                fprintf(ofp3, "%d %s ", identsym, charBuffer );
                fprintf(ofp4, "%d %s ", identsym, charBuffer );

                if (printLex)
                    printf("%d %s ", identsym, charBuffer);
            }

            //Checks to see if the word is equivalent to the const key word
            else if (strcmp(charBuffer, "const") == 0){

                fprintf(ofp2, "const\t%d\n", constsym);
                fprintf(ofp3, "%d ", constsym);
                fprintf(ofp4, "%d ", constsym);

                if (printLex)
                    printf("%d ", constsym);
            }

            else if (strcmp(charBuffer, "var") == 0){

                fprintf(ofp2, "var\t%d\n", varsym);
                fprintf(ofp3, "%d ", varsym);
                fprintf(ofp4, "%d ", varsym);

                if (printLex)
                    printf("%d ", varsym);

            }

            else if (strcmp(charBuffer, "procedure") == 0){

                fprintf(ofp2, "procedure\t%d\n", procsym);
                fprintf(ofp3, "%d ", procsym);
                fprintf(ofp4, "%d ", procsym);

                if (printLex)
                    printf("%d ", procsym);
            }

            else if (strcmp(charBuffer, "call") ==0){

                fprintf(ofp2, "call\t%d\n", callsym);
                fprintf(ofp3, "%d ", callsym);
                fprintf(ofp4, "%d ", callsym);

                if (printLex)
                    printf("%d ", callsym);
            }

            else if (strcmp(charBuffer, "begin") == 0){

                fprintf(ofp2, "begin\t%d\n", beginsym);
                fprintf(ofp3, "%d ", beginsym);
                fprintf(ofp4, "%d ", beginsym);

                if (printLex)
                    printf("%d ", beginsym);
            }

            else if (strcmp(charBuffer, "end") == 0){

                fprintf(ofp2, "end\t%d\n", endsym);
                fprintf(ofp3, "%d ", endsym);
                fprintf(ofp4, "%d ", endsym);

                if (printLex)
                    printf("%d ", endsym);
            }

            else if (strcmp(charBuffer, "if") == 0){

                fprintf(ofp2, "if\t%d\n", ifsym);
                fprintf(ofp3, "%d ", ifsym );
                fprintf(ofp4, "%d ", ifsym);

                if (printLex)
                    printf("%d ", ifsym);
            }

            else if (strcmp(charBuffer, "then") == 0){
                fprintf(ofp2, "then\t%d\n", thensym);
                fprintf(ofp3, "%d ",thensym );
                fprintf(ofp4, "%d ",thensym );

                if (printLex)
                    printf("%d ", thensym );
            }

            else if (strcmp(charBuffer, "else") == 0){

                fprintf(ofp2, "else\t%d\n", elsesym );
                fprintf(ofp3, "%d ", elsesym);
                fprintf(ofp4, "%d ", elsesym );

                if (printLex)
                    printf("%d ", elsesym);
            }

            else if (strcmp(charBuffer, "while") == 0){

                fprintf(ofp2, "while\t%d\n", whilesym );
                fprintf(ofp3, "%d ",whilesym );
                fprintf(ofp4, "%d ",whilesym );

                if (printLex)
                    printf("%d ", whilesym);
            }

            else if (strcmp(charBuffer, "do") == 0){

                fprintf(ofp2, "do\t%d\n", dosym);
                fprintf(ofp3, "%d ",dosym );
                fprintf(ofp4, "%d ",dosym );

                if (printLex)
                    printf("%d ", dosym);
            }

            else if (strcmp(charBuffer, "read") == 0){

                fprintf(ofp2, "read\t%d\n", readsym );
                fprintf(ofp3, "%d ", readsym );
                fprintf(ofp4, "%d ", readsym );

                if (printLex)
                    printf("%d ", readsym);
            }

            else if (strcmp(charBuffer, "write") == 0){

                fprintf(ofp2, "write\t%d\n", writesym );
                fprintf(ofp3, "%d ", writesym );
                fprintf(ofp4, "%d ", writesym );

                if (printLex)
                    printf("%d ", writesym);
            }

            else if (strcmp(charBuffer, "odd") == 0){

                fprintf(ofp2, "odd\t%d\n", oddsym );
                fprintf(ofp3, "%d ", oddsym );
                fprintf(ofp4, "%d ", oddsym);

                if (printLex)
                    printf("%d ", oddsym);
            }

            //The candidate is not a keyword, so we can just print it to the output file.
            else {

                fprintf(ofp2, "%s\t%d\n", charBuffer, identsym);
                fprintf(ofp3, "%d %s ", identsym, charBuffer );
                fprintf(ofp4, "%d %s ", identsym, charBuffer );

                if (printLex)
                    printf("%d %s ", identsym, charBuffer );

            }


            //Puts the chracter back into the stream because we don't need it anymore
            ungetc(temp, ifp);

        }

        //This is a number
        else if (isdigit(temp)){

            do{

                count++;

                if (count > MAX_NUM_LENGTH){

                    printf("The number is too long");
                    exit(0);
                }

                //Adds the character to the value
                value = 10 * value + temp - '0';


            }while (isdigit(temp = getc(ifp)));

            //Is there is a letter after the digits, it's an illegal variable
            if (isalpha(temp)){

                printf("Variable does not start with letter: %d", value);
                exit(0);
            }

            //Otherwise, just print the number out
            else{
                ungetc(temp, ifp);
                fprintf(ofp2, "%d\t%d\n", value, numbersym);
                fprintf(ofp3, "%d %d ",numbersym, value);
                fprintf(ofp4, "%d %d ",numbersym, value);

                if (printLex)
                    printf("%d %d ",numbersym, value);
            }



        }

        //Checks to see if the character is one of the valid symbols
        else if (temp == '+'){

           fprintf(ofp2, "+\t%d\n", plussym);
           fprintf(ofp3, "%d ", plussym);
           fprintf(ofp4, "%d ", plussym);

           if (printLex)
            printf("%d ", plussym);
        }

        else if (temp == '-'){

            fprintf(ofp2, "-\t%d\n", minussym);
            fprintf(ofp3, "%d ", minussym);
            fprintf(ofp4, "%d ", minussym);

            if (printLex)
                printf("%d ", minussym);
        }

        else if (temp == '*'){

            fprintf(ofp2, "*\t%d\n", multsym);
            fprintf(ofp3, "%d ", multsym);
            fprintf(ofp4, "%d ", multsym);

            if (printLex)
                printf("%d ", multsym);
        }

        else if (temp == '/'){

            fprintf(ofp2, "/\t%d\n", slashsym);
            fprintf(ofp3, "%d ", slashsym);
            fprintf(ofp4, "%d ", slashsym);

            if (printLex)
                printf("%d ", slashsym);
        }

        else if (temp == '('){

            fprintf(ofp2, "(\t%d\n", lparentsym);
            fprintf(ofp3, "%d ", lparentsym);
            fprintf(ofp4, "%d ", lparentsym);

            if (printLex)
                printf("%d ", lparentsym);
        }

        else if (temp == ')'){

            fprintf(ofp2, ")\t%d\n", rparentsym);
            fprintf(ofp3, "%d ", rparentsym);
            fprintf(ofp4, "%d ", rparentsym);

            if (printLex)
                printf("%d ", rparentsym);
        }

        else if (temp == '='){

            fprintf(ofp2, "=\t%d\n", eqsym);
            fprintf(ofp3, "%d ", eqsym);
            fprintf(ofp4, "%d ", eqsym);

            if (printLex)
                printf("%d ", eqsym);
        }

        else if (temp == ','){

            fprintf(ofp2, ",\t%d\n", commasym);
            fprintf(ofp3, "%d ", commasym);
            fprintf(ofp4, "%d ", commasym);

            if (printLex)
                printf("%d ", commasym);
        }

        else if (temp == '.'){

            fprintf(ofp2, ".\t%d\n", periodsym);
            fprintf(ofp3, "%d ", periodsym);
            fprintf(ofp4, "%d ", periodsym);

            if (printLex)
                printf("%d ", periodsym );
        }

        else if (temp == '<'){

            temp = getc(ifp);

            if (temp == '='){

                fprintf(ofp2, "<=\t%d\n", leqsym);
                fprintf(ofp3, "%d ", leqsym);
                fprintf(ofp4, "%d ", leqsym);

                if (printLex)
                    printf("%d ", leqsym);
            }

            else if (temp == '>'){

                fprintf(ofp2, "<>\t%d\n", neqsym);
                fprintf(ofp3, "%d ", neqsym);
                fprintf(ofp4, "%d ", neqsym);

                if (printLex)
                    printf("%d ", neqsym);

            }

            else{

                fprintf(ofp2, "<\t%d\n", lessym);
                fprintf(ofp3, "%d ", lessym);
                fprintf(ofp4, "%d ", lessym);

                if (printLex)
                    printf("%d ", lessym);
                

            }


        }

        else if (temp == '>'){

            temp = getc(ifp);

            if (temp == '='){

                fprintf(ofp2, ">=\t%d\n",geqsym);
                fprintf(ofp3, "%d ", geqsym);
                fprintf(ofp4, "%d ", geqsym);

                if (printLex)
                    printf("%d ", geqsym);
            }

            else{

                ungetc(temp, ifp);
                fprintf(ofp2, ">\t%d\n", gtrsym);
                fprintf(ofp3, "%d ", gtrsym);
                fprintf(ofp4, "%d ", gtrsym);

                if (printLex)
                    printf("%d ", geqsym);

            }

        }

        else if (temp == ';'){

            fprintf(ofp2, ";\t%d\n", semicolonsym);
            fprintf(ofp3, "%d ", semicolonsym);
            fprintf(ofp4, "%d ", semicolonsym);

            if (printLex)
                printf("%d ", semicolonsym );
        }

        else if (temp == ':'){

            temp = getc(ifp);

            if (temp == '='){
                fprintf(ofp2, ":=\t%d\n", becomessym);
                fprintf(ofp3, "%d ", becomessym);
                fprintf(ofp4, "%d ", becomessym);

                if (printLex)
                    printf("%d ",becomessym);
            }

            else {

                printf("Error: We do not recognize this symbol. Did you mean to put := ?");
                exit(0);
            }

        }

        else if (isspace(temp))
            continue;

        else {
            printf("Error: Invalid Symbol %c", (char)temp);
            exit(0);
        }




    }

    fprintf(ofp4, "\n\n");
    printf("\n");

    fclose(ifp);
    fclose(ofp2);
    fclose(ofp3);
    fclose(ofp4);


}

int main(int argc, char *argv[])
{
    int i;
    int printLex = 0;

    //Checks to see if the number of arguments is correct
    if (argc > 2){

        printf("Invalid number of arguments: %d", argc);

        exit(1);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-l") == 0){

            printLex = 1;
            continue;

            
        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );
            exit(1);


        }

    }

    //Checks to see if the file exists
    if (!(access( "input.txt", F_OK ) != -1)){
        printf("input.txt does not exist in the current directory\n");
        exit(1);
    }

    //Used to open input and output files
    FILE *ifp, *ofp1, *ofp2, *ofp3, *ofp4;
    ifp = fopen("input.txt", "r");
    ofp1 = fopen("cleaninput.txt", "w");
    ofp2 = fopen("lexemetable.txt", "w");
    ofp3 = fopen("lexemelist.txt", "w");
    ofp4 = fopen("output.txt", "w");

    removeComments(ifp, ofp1, ofp4);

    //We read from the comment stripped input file now
    ifp = fopen("cleaninput.txt", "r");

    convertToToken(ifp, ofp2, ofp3, ofp4, printLex);

    return 0;

}
