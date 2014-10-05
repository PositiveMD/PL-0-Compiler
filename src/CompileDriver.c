/*
* Alvin Lam
* Anthony Dinh
* COP 3402 : System Software
* October 3rd 2014
* Assignment 3 : Tiny PL/0 Parser and Code Generator
*/

#include "CompilerDriver.h"

int main(int argc, char *argv[]){

    int i;

    //These variables determine whether to print the Lexemes, Assembly code, or the Stack Trace to the screen
    int printLex = 0;
    int printPars = 0;
    int printStack = 0;

    //Checks to see if the number of arguments is correct
    if (argc > 4){

        printf("Invalid number of arguments: %d", argc);

        exit(0);
    }

    //Checks to see what arguments are passed
    for (i = 1; i < argc; i++){

        if (strcmp(argv[i],"-l") == 0){

            printLex = 1;
            continue;
        }

        if (strcmp(argv[i], "-a") == 0){

            printPars = 1;
            continue;
        }

        if (strcmp(argv[i], "-v") == 0){

            printStack = 1;
            continue;
        }

        else {

            printf("Invalid argument: %s \nArgument number: %d\n", argv[i], i );

            
        }

    }

    startScanner(printLex);














    return 0;
}
