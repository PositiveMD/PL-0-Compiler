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
    for (i = 0; i < argc; i++){

        if (argv[i] == 'l'){

            printLex = 1;
            return;
        }

        if (argv[i] == 'a'){

            printPars = 1;
            return;
        }

        if (argv[i] == 'v'){

            printStack = 1;
            return;
        }

        else {

            printf("Invalid argument: %c", argv[i]);

            
        }

    }

    startScanner(printLex);














    return 0;
}
